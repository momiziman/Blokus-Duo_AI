from __future__ import annotations

import json
from dataclasses import dataclass, replace
from datetime import datetime
from pathlib import Path
from typing import Callable

import numpy as np
import torch

from .checkpoint import load_checkpoint, save_checkpoint
from .config import AlphaZeroConfig
from .model_arena import evaluate_candidate
from .network import PolicyValueNetwork
from .replay_storage import load_replay_buffer, save_replay_buffer
from .self_play import SelfPlayRunner
from .trainer import Trainer


STATE_SCHEMA_VERSION = 1


@dataclass(slots=True)
class IterationResult:
    iteration: int
    promoted: bool
    candidate_point_rate: float
    replay_size: int
    losses: dict[str, float]


class AlphaZeroLearningLoop:
    """Resumable self-play, training, arena, and promotion loop."""

    def __init__(
        self,
        config: AlphaZeroConfig,
        device: torch.device | str,
        game_factory,
        model_factory=PolicyValueNetwork,
        progress: Callable[[str], None] = print,
    ) -> None:
        self.config = config
        self.device = torch.device(device)
        self.game_factory = game_factory
        self.model_factory = model_factory
        self.progress = progress
        self._validate_config()

    def _validate_config(self) -> None:
        if self.config.training_iterations <= 0:
            raise ValueError("training_iterations must be positive")
        if self.config.iteration_self_play_games <= 0:
            raise ValueError("iteration_self_play_games must be positive")
        if self.config.arena_games <= 0 or self.config.arena_games % 2 != 0:
            raise ValueError("arena_games must be a positive even number")
        if not 0.0 <= self.config.promotion_threshold <= 1.0:
            raise ValueError("promotion_threshold must be between 0 and 1")
        if self.config.iteration_simulations <= 0:
            raise ValueError("iteration_simulations must be positive")
        if self.config.iteration_training_epochs <= 0:
            raise ValueError("iteration_training_epochs must be positive")
        if self.config.replay_capacity <= 0:
            raise ValueError("replay_capacity must be positive")

    def _new_model(self) -> torch.nn.Module:
        return self.model_factory(self.config).to(self.device)

    def _iteration_settings(self) -> dict[str, int | float]:
        return {
            "self_play_games": self.config.iteration_self_play_games,
            "arena_games": self.config.arena_games,
            "simulations": self.config.iteration_simulations,
            "training_epochs": self.config.iteration_training_epochs,
            "promotion_threshold": self.config.promotion_threshold,
            "hidden_channels": self.config.hidden_channels,
            "residual_blocks": self.config.residual_blocks,
        }

    @staticmethod
    def _write_json_atomic(path: Path, value: dict) -> None:
        path.parent.mkdir(parents=True, exist_ok=True)
        temporary = path.with_suffix(path.suffix + ".tmp")
        temporary.write_text(
            json.dumps(value, ensure_ascii=False, indent=2), encoding="utf-8"
        )
        temporary.replace(path)

    def _new_state(self) -> dict:
        return {
            "schema_version": STATE_SCHEMA_VERSION,
            "completed_iterations": 0,
            "best_step": 0,
            "total_self_play_games": 0,
            "in_progress": None,
        }

    def _load_or_initialize(self) -> tuple[dict, torch.nn.Module]:
        state_path = Path(self.config.training_state_path)
        best_path = Path(self.config.best_checkpoint_path)
        replay_path = Path(self.config.replay_path)
        best_model = self._new_model()

        if state_path.exists():
            state = json.loads(state_path.read_text(encoding="utf-8"))
            if state.get("schema_version") != STATE_SCHEMA_VERSION:
                raise ValueError("unsupported iterative-training state schema")
            if not best_path.exists():
                raise FileNotFoundError(
                    f"training state exists but best checkpoint is missing: {best_path}"
                )
            checkpoint_step = load_checkpoint(
                best_path, best_model, map_location=self.device
            )
            if checkpoint_step != int(state.get("best_step", -1)):
                raise RuntimeError(
                    "best checkpoint step does not match the training state"
                )
        else:
            if best_path.exists() or replay_path.exists():
                raise RuntimeError(
                    "iterative artifacts exist without training_state.json; "
                    "move them aside or restore the missing state file"
                )
            state = self._new_state()
            save_checkpoint(best_path, best_model, None, self.config, step=0)
            self._write_json_atomic(state_path, state)

        in_progress = state.get("in_progress")
        if in_progress is not None:
            expected = self._iteration_settings()
            if in_progress.get("settings") != expected:
                raise RuntimeError(
                    "configuration changed during an unfinished iteration; "
                    "resume it with the original settings"
                )
            if self.config.training_iterations < int(in_progress["iteration"]):
                raise RuntimeError(
                    "the target iteration is below the unfinished iteration"
                )
        replay_required = int(state.get("completed_iterations", 0)) > 0 or (
            in_progress is not None and in_progress.get("phase") != "started"
        )
        if replay_required and not replay_path.exists():
            raise FileNotFoundError(
                f"training state requires a replay file: {replay_path}"
            )
        return state, best_model

    def _save_state(self, state: dict) -> None:
        self._write_json_atomic(Path(self.config.training_state_path), state)

    def _candidate_path(self, iteration: int) -> Path:
        return Path(self.config.iteration_checkpoint_dir) / (
            f"iteration_{iteration:04d}.pt"
        )

    def _append_log_once(self, record: dict) -> None:
        destination = Path(self.config.iteration_log_path)
        destination.parent.mkdir(parents=True, exist_ok=True)
        if destination.exists():
            for line in destination.read_text(encoding="utf-8").splitlines():
                if line and json.loads(line).get("iteration") == record["iteration"]:
                    return
        with destination.open("a", encoding="utf-8", newline="\n") as output:
            output.write(json.dumps(record, ensure_ascii=False) + "\n")

    @staticmethod
    def _elapsed_seconds(started_at: str) -> float:
        started = datetime.fromisoformat(started_at)
        return (datetime.now().astimezone() - started).total_seconds()

    def _start_iteration(self, state: dict, iteration: int) -> dict:
        in_progress = {
            "iteration": iteration,
            "phase": "started",
            "started_at": datetime.now().astimezone().isoformat(),
            "settings": self._iteration_settings(),
        }
        state["in_progress"] = in_progress
        self._save_state(state)
        return in_progress

    def run(self) -> list[IterationResult]:
        state, best_model = self._load_or_initialize()
        replay = load_replay_buffer(
            self.config.replay_path, self.config.replay_capacity
        )
        completed_now: list[IterationResult] = []

        while state["completed_iterations"] < self.config.training_iterations:
            iteration = state["completed_iterations"] + 1
            iteration_config = replace(
                self.config,
                simulations=self.config.iteration_simulations,
                training_epochs=self.config.iteration_training_epochs,
            )
            in_progress = state.get("in_progress")
            if in_progress is None:
                in_progress = self._start_iteration(state, iteration)
            elif in_progress.get("iteration") != iteration:
                raise RuntimeError("training state contains an unexpected iteration")

            self.progress(
                f"iteration={iteration}/{self.config.training_iterations} "
                f"phase={in_progress['phase']} replay={len(replay)}"
            )

            if in_progress["phase"] == "started":
                runner = SelfPlayRunner(
                    best_model,
                    iteration_config,
                    self.device,
                    self.game_factory,
                    rng=np.random.default_rng(self.config.seed + iteration * 10_000),
                )
                game_summaries = []
                positions_added = 0
                for game_index in range(self.config.iteration_self_play_games):
                    examples, scores = runner.play_game()
                    replay.add_game(examples)
                    positions_added += len(examples)
                    game_summaries.append(
                        {
                            "game": game_index + 1,
                            "positions": len(examples),
                            "scores": list(scores),
                        }
                    )
                    self.progress(
                        f"  self_play={game_index + 1}/"
                        f"{self.config.iteration_self_play_games} "
                        f"positions={len(examples)} scores={scores}"
                    )
                save_replay_buffer(self.config.replay_path, replay)
                in_progress.update(
                    {
                        "phase": "self_play_complete",
                        "games": game_summaries,
                        "positions_added": positions_added,
                        "replay_size": len(replay),
                    }
                )
                self._save_state(state)

            candidate = self._new_model()
            trainer = Trainer(
                candidate,
                iteration_config,
                self.device,
                rng=np.random.default_rng(self.config.seed + iteration * 10_000 + 1),
            )
            candidate_path = self._candidate_path(iteration)

            if in_progress["phase"] == "self_play_complete":
                load_checkpoint(
                    self.config.best_checkpoint_path,
                    candidate,
                    optimizer=trainer.optimizer,
                    map_location=self.device,
                )
                losses = trainer.train(replay)
                save_checkpoint(
                    candidate_path,
                    candidate,
                    trainer.optimizer,
                    iteration_config,
                    step=iteration,
                )
                in_progress.update(
                    {"phase": "candidate_trained", "losses": losses}
                )
                self._save_state(state)
                self.progress(f"  trained losses={losses}")
            else:
                if not candidate_path.exists():
                    raise FileNotFoundError(
                        f"candidate checkpoint is missing: {candidate_path}"
                    )
                load_checkpoint(
                    candidate_path,
                    candidate,
                    optimizer=trainer.optimizer,
                    map_location=self.device,
                )

            if in_progress["phase"] == "candidate_trained":
                arena_config = replace(
                    iteration_config,
                    seed=self.config.seed + iteration * 10_000 + 2,
                )
                arena = evaluate_candidate(
                    candidate,
                    best_model,
                    arena_config,
                    self.device,
                    self.game_factory,
                )
                promoted = (
                    arena.candidate_point_rate >= self.config.promotion_threshold
                )
                in_progress.update(
                    {
                        "phase": "arena_complete",
                        "arena": arena.as_dict(),
                        "promoted": promoted,
                    }
                )
                self._save_state(state)
                self.progress(
                    f"  arena candidate={arena.candidate_wins} "
                    f"best={arena.best_wins} draws={arena.draws} "
                    f"point_rate={arena.candidate_point_rate:.3f}"
                )

            if in_progress["phase"] != "arena_complete":
                raise RuntimeError(f"unknown iteration phase: {in_progress['phase']}")

            promoted = bool(in_progress["promoted"])
            best_step = iteration if promoted else int(state["best_step"])
            if promoted:
                save_checkpoint(
                    self.config.best_checkpoint_path,
                    candidate,
                    trainer.optimizer,
                    iteration_config,
                    step=iteration,
                )
                best_model.load_state_dict(candidate.state_dict())

            record = {
                "iteration": iteration,
                "started_at": in_progress["started_at"],
                "wall_elapsed_seconds": self._elapsed_seconds(
                    in_progress["started_at"]
                ),
                "settings": in_progress["settings"],
                "games": in_progress["games"],
                "positions_added": in_progress["positions_added"],
                "replay_size": in_progress["replay_size"],
                "losses": in_progress["losses"],
                "arena": in_progress["arena"],
                "promoted": promoted,
                "best_step": best_step,
            }
            self._append_log_once(record)
            state.update(
                {
                    "completed_iterations": iteration,
                    "best_step": best_step,
                    "total_self_play_games": state["total_self_play_games"]
                    + self.config.iteration_self_play_games,
                    "in_progress": None,
                }
            )
            self._save_state(state)
            completed_now.append(
                IterationResult(
                    iteration=iteration,
                    promoted=promoted,
                    candidate_point_rate=float(
                        in_progress["arena"]["candidate_point_rate"]
                    ),
                    replay_size=int(in_progress["replay_size"]),
                    losses=dict(in_progress["losses"]),
                )
            )
            self.progress(
                f"iteration={iteration} complete promoted={promoted} "
                f"best_step={best_step}"
            )

        if not completed_now:
            self.progress(
                f"training already complete at iteration "
                f"{state['completed_iterations']}"
            )
        return completed_now
