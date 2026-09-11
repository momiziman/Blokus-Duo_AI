from __future__ import annotations

from collections import deque
from dataclasses import dataclass

import numpy as np


@dataclass(slots=True)
class TrainingExample:
    observation: np.ndarray
    policy: np.ndarray
    value: float


class ReplayBuffer:
    def __init__(self, capacity: int) -> None:
        self.examples: deque[TrainingExample] = deque(maxlen=capacity)

    def add_game(self, examples: list[TrainingExample]) -> None:
        self.examples.extend(examples)

    def add_examples(self, examples: list[TrainingExample]) -> None:
        self.examples.extend(examples)

    def to_list(self) -> list[TrainingExample]:
        return list(self.examples)

    def sample(self, batch_size: int, rng: np.random.Generator) -> list[TrainingExample]:
        if not self.examples:
            raise ValueError("cannot sample from an empty replay buffer")
        count = min(batch_size, len(self.examples))
        indices = rng.choice(len(self.examples), size=count, replace=False)
        values = list(self.examples)
        return [values[int(index)] for index in indices]

    def __len__(self) -> int:
        return len(self.examples)
