from __future__ import annotations

import numpy as np
import torch
from torch.nn import functional as F

from .config import AlphaZeroConfig
from .replay_buffer import ReplayBuffer


class Trainer:
    def __init__(
        self,
        model: torch.nn.Module,
        config: AlphaZeroConfig,
        device: torch.device | str,
        rng: np.random.Generator | None = None,
    ) -> None:
        self.model = model
        self.config = config
        self.device = torch.device(device)
        self.optimizer = torch.optim.AdamW(
            model.parameters(),
            lr=config.learning_rate,
            weight_decay=config.weight_decay,
        )
        self.rng = rng or np.random.default_rng(config.seed)

    def train(self, replay: ReplayBuffer) -> dict[str, float]:
        self.model.train()
        totals = {"loss": 0.0, "policy_loss": 0.0, "value_loss": 0.0}
        steps = 0
        batches_per_epoch = max(1, len(replay) // self.config.batch_size)

        for _ in range(self.config.training_epochs):
            for _ in range(batches_per_epoch):
                batch = replay.sample(self.config.batch_size, self.rng)
                observations = torch.tensor(
                    np.stack([example.observation for example in batch]),
                    dtype=torch.float32,
                    device=self.device,
                )
                targets_policy = torch.tensor(
                    np.stack([example.policy for example in batch]),
                    dtype=torch.float32,
                    device=self.device,
                )
                targets_value = torch.tensor(
                    [example.value for example in batch],
                    dtype=torch.float32,
                    device=self.device,
                )

                logits, values = self.model(observations)
                policy_loss = -(
                    targets_policy * F.log_softmax(logits, dim=1)
                ).sum(dim=1).mean()
                value_loss = F.mse_loss(values, targets_value)
                loss = policy_loss + value_loss

                self.optimizer.zero_grad(set_to_none=True)
                loss.backward()
                self.optimizer.step()

                totals["loss"] += float(loss.item())
                totals["policy_loss"] += float(policy_loss.item())
                totals["value_loss"] += float(value_loss.item())
                steps += 1

        return {name: value / steps for name, value in totals.items()}
