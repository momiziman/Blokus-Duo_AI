from __future__ import annotations

from dataclasses import dataclass

import numpy as np
import torch

from .config import AlphaZeroConfig
from .puct_mcts import PUCTSearch


@dataclass(slots=True)
class ModelArenaResult:
    candidate_wins: int = 0
    best_wins: int = 0
    draws: int = 0
    candidate_score: int = 0
    best_score: int = 0

    @property
    def games(self) -> int:
        return self.candidate_wins + self.best_wins + self.draws

    @property
    def candidate_point_rate(self) -> float:
        if self.games == 0:
            return 0.0
        return (self.candidate_wins + 0.5 * self.draws) / self.games

    def as_dict(self) -> dict[str, int | float]:
        return {
            "candidate_wins": self.candidate_wins,
            "best_wins": self.best_wins,
            "draws": self.draws,
            "candidate_score": self.candidate_score,
            "best_score": self.best_score,
            "candidate_point_rate": self.candidate_point_rate,
        }


def evaluate_candidate(
    candidate: torch.nn.Module,
    best: torch.nn.Module,
    config: AlphaZeroConfig,
    device: torch.device | str,
    game_factory,
    games: int | None = None,
) -> ModelArenaResult:
    """Play a color-balanced, noise-free candidate-versus-best arena."""
    game_count = config.arena_games if games is None else games
    if game_count <= 0 or game_count % 2 != 0:
        raise ValueError("model arena games must be a positive even number")

    result = ModelArenaResult()
    candidate_rng = np.random.default_rng(config.seed + 20_001)
    best_rng = np.random.default_rng(config.seed + 20_002)

    for game_index in range(game_count):
        state = game_factory()
        candidate_player = game_index % 2

        for _ in range(config.max_game_length):
            if state.is_terminal:
                break
            is_candidate_turn = state.current_player == candidate_player
            model = candidate if is_candidate_turn else best
            rng = candidate_rng if is_candidate_turn else best_rng
            search = PUCTSearch(model, config, device, rng=rng)
            root = search.run(state, add_noise=False)
            policy = search.policy(root, temperature=0.0)
            state.play_action(search.select_action(policy, sample=False))
        else:
            raise RuntimeError("model arena game exceeded max_game_length")

        scores = tuple(state.scores)
        candidate_score = scores[candidate_player]
        best_score = scores[1 - candidate_player]
        result.candidate_score += candidate_score
        result.best_score += best_score
        if candidate_score > best_score:
            result.candidate_wins += 1
        elif candidate_score < best_score:
            result.best_wins += 1
        else:
            result.draws += 1

    return result
