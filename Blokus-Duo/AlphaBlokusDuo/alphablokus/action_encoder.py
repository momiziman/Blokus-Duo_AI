from __future__ import annotations

BOARD_SIZE = 14
PIECE_COUNT = 21
ORIENTATIONS = 8
SPATIAL_ACTIONS = PIECE_COUNT * ORIENTATIONS * BOARD_SIZE * BOARD_SIZE
PASS_ACTION = SPATIAL_ACTIONS
ACTION_SIZE = SPATIAL_ACTIONS + 1


def components_to_action(piece: int, rotation: int, x: int, y: int) -> int:
    """Encode zero-based piece/rotation and one-based board coordinates."""
    if not 0 <= piece < PIECE_COUNT:
        raise ValueError("piece must be in [0, 20]")
    if not 0 <= rotation < ORIENTATIONS:
        raise ValueError("rotation must be in [0, 7]")
    if not 1 <= x <= BOARD_SIZE or not 1 <= y <= BOARD_SIZE:
        raise ValueError("x and y must be in [1, 14]")
    return (((piece * ORIENTATIONS + rotation) * BOARD_SIZE + (y - 1))
            * BOARD_SIZE + (x - 1))


def action_to_components(action: int) -> tuple[int, int, int, int] | None:
    """Return (piece, rotation, x, y), or None for PASS."""
    if action == PASS_ACTION:
        return None
    if not 0 <= action < SPATIAL_ACTIONS:
        raise ValueError("action is outside the policy space")
    value = action
    x = value % BOARD_SIZE + 1
    value //= BOARD_SIZE
    y = value % BOARD_SIZE + 1
    value //= BOARD_SIZE
    rotation = value % ORIENTATIONS
    piece = value // ORIENTATIONS
    return piece, rotation, x, y
