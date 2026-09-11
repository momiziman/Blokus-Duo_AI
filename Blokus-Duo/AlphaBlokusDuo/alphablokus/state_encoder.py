from __future__ import annotations

import torch

from .config import AlphaZeroConfig


def encode_state(state, config: AlphaZeroConfig, device: torch.device | str = "cpu") -> torch.Tensor:
    values = torch.tensor(state.observation(), dtype=torch.float32, device=device)
    return values.reshape(
        config.observation_channels,
        config.board_size,
        config.board_size,
    )
