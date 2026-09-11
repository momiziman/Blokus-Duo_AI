from __future__ import annotations

import torch
from torch import nn

from .config import AlphaZeroConfig


class ResidualBlock(nn.Module):
    def __init__(self, channels: int) -> None:
        super().__init__()
        self.layers = nn.Sequential(
            nn.Conv2d(channels, channels, 3, padding=1, bias=False),
            nn.BatchNorm2d(channels),
            nn.ReLU(inplace=True),
            nn.Conv2d(channels, channels, 3, padding=1, bias=False),
            nn.BatchNorm2d(channels),
        )
        self.relu = nn.ReLU(inplace=True)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.relu(x + self.layers(x))


class PolicyValueNetwork(nn.Module):
    """Compact residual policy/value network for the AlphaZero MVP."""

    def __init__(self, config: AlphaZeroConfig) -> None:
        super().__init__()
        channels = config.hidden_channels
        self.action_size = config.action_size
        self.board_size = config.board_size

        self.stem = nn.Sequential(
            nn.Conv2d(config.observation_channels, channels, 3, padding=1, bias=False),
            nn.BatchNorm2d(channels),
            nn.ReLU(inplace=True),
        )
        self.tower = nn.Sequential(
            *(ResidualBlock(channels) for _ in range(config.residual_blocks))
        )

        # 21 pieces * 8 orientations becomes one spatial logit channel per
        # piece/orientation. This avoids a prohibitively large fully connected head.
        self.policy_board = nn.Conv2d(channels, 21 * 8, kernel_size=1)
        self.policy_pass = nn.Linear(channels, 1)

        self.value_head = nn.Sequential(
            nn.Conv2d(channels, 1, kernel_size=1, bias=False),
            nn.BatchNorm2d(1),
            nn.ReLU(inplace=True),
            nn.Flatten(),
            nn.Linear(config.board_size * config.board_size, 128),
            nn.ReLU(inplace=True),
            nn.Linear(128, 1),
            nn.Tanh(),
        )

    def forward(self, x: torch.Tensor) -> tuple[torch.Tensor, torch.Tensor]:
        features = self.tower(self.stem(x))
        board_logits = self.policy_board(features).flatten(start_dim=1)
        pooled = features.mean(dim=(2, 3))
        pass_logit = self.policy_pass(pooled)
        policy_logits = torch.cat((board_logits, pass_logit), dim=1)
        value = self.value_head(features).squeeze(1)
        return policy_logits, value
