from __future__ import annotations

from dataclasses import dataclass

import numpy as np
import torch

from .config import AlphaZeroConfig
from .puct_mcts import PUCTSearch


@dataclass(slots=True)
class ArenaResult:
    alpha_wins: int = 0
    baseline_wins: int = 0
    draws: int = 0
    alpha_score: int = 0
    baseline_score: int = 0


def evaluate_against_baseline(
    model: torch.nn.Module,
    config: AlphaZeroConfig,
    device: torch.device | str,
    game_factory,
    baseline: str = "random",
    games: int = 4,
    baseline_iterations: int = 100,
) -> ArenaResult:
    result = ArenaResult()
    rng = np.random.default_rng(config.seed + 1)

    for game_index in range(games):
        state = game_factory()
        alpha_player = game_index % 2

        for _ in range(config.max_game_length):
            if state.is_terminal:
                break
            if state.current_player == alpha_player:
                search = PUCTSearch(model, config, device, rng=rng)
                root = search.run(state, add_noise=False)
                policy = search.policy(root, temperature=0.0)
                state.play_action(search.select_action(policy, sample=False))
            else:
                move = state.select_baseline(
                    baseline, baseline_iterations, 10, config.seed + game_index
                )
                state.play(move)
        else:
            raise RuntimeError("arena game exceeded max_game_length")

        scores = tuple(state.scores)
        alpha_score = scores[alpha_player]
        baseline_score = scores[1 - alpha_player]
        result.alpha_score += alpha_score
        result.baseline_score += baseline_score
        if alpha_score > baseline_score:
            result.alpha_wins += 1
        elif alpha_score < baseline_score:
            result.baseline_wins += 1
        else:
            result.draws += 1

    return result
