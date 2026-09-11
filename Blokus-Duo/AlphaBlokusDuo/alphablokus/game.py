from __future__ import annotations

try:
    from ._alpha_game import (
        ACTION_SIZE,
        BOARD_SIZE,
        OBSERVATION_CHANNELS,
        PASS_ACTION,
        AlphaGameState,
        Move,
    )
except ImportError as exc:
    raise ImportError(
        "The C++ game bridge is not built. Run `python build_extension.py` "
        "inside AlphaBlokusDuo first."
    ) from exc

__all__ = [
    "ACTION_SIZE",
    "BOARD_SIZE",
    "OBSERVATION_CHANNELS",
    "PASS_ACTION",
    "AlphaGameState",
    "Move",
]
