from __future__ import annotations

import numpy as np
import torch

from .config import AlphaZeroConfig
from .puct_mcts import PUCTSearch
from .replay_buffer import TrainingExample


class SelfPlayRunner:
    def __init__(
        self,
        model: torch.nn.Module,
        config: AlphaZeroConfig,
        device: torch.device | str,
        game_factory,
        rng: np.random.Generator | None = None,
    ) -> None:
        self.model = model
        self.config = config
        self.device = torch.device(device)
        self.game_factory = game_factory
        self.rng = rng or np.random.default_rng(config.seed)

    def play_game(self) -> tuple[list[TrainingExample], tuple[int, int]]:
        state = self.game_factory()
        history: list[tuple[np.ndarray, np.ndarray, int]] = []

        for move_number in range(self.config.max_game_length):
            if state.is_terminal:
                break
            search = PUCTSearch(
                self.model, self.config, self.device, rng=self.rng
            )
            root = search.run(state, add_noise=True)
            temperature = 1.0 if move_number < self.config.temperature_moves else 0.0
            policy = search.policy(root, temperature)
            observation = np.asarray(state.observation(), dtype=np.float32).reshape(
                self.config.observation_channels,
                self.config.board_size,
                self.config.board_size,
            )
            history.append((observation, policy, state.current_player))
            action = search.select_action(policy, sample=temperature > 0.0)
            state.play_action(action)
        else:
            raise RuntimeError("self-play exceeded max_game_length")

        examples = [
            TrainingExample(
                observation=observation,
                policy=policy,
                value=float(state.terminal_value(player)),
            )
            for observation, policy, player in history
        ]
        return examples, tuple(state.scores)
