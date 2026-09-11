from __future__ import annotations

from dataclasses import asdict
from pathlib import Path

import torch

from .config import AlphaZeroConfig


def save_checkpoint(
    path: str | Path,
    model: torch.nn.Module,
    optimizer: torch.optim.Optimizer | None,
    config: AlphaZeroConfig,
    step: int,
) -> None:
    destination = Path(path)
    destination.parent.mkdir(parents=True, exist_ok=True)
    temporary = destination.with_suffix(destination.suffix + ".tmp")
    torch.save(
        {
            "model": model.state_dict(),
            "optimizer": optimizer.state_dict() if optimizer else None,
            "config": asdict(config),
            "step": step,
        },
        temporary,
    )
    temporary.replace(destination)


def load_checkpoint(
    path: str | Path,
    model: torch.nn.Module,
    optimizer: torch.optim.Optimizer | None = None,
    map_location: str | torch.device = "cpu",
) -> int:
    data = torch.load(path, map_location=map_location, weights_only=True)
    model.load_state_dict(data["model"])
    if optimizer is not None and data.get("optimizer") is not None:
        optimizer.load_state_dict(data["optimizer"])
    return int(data.get("step", 0))
