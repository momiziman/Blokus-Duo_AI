from __future__ import annotations

from dataclasses import dataclass


@dataclass(slots=True)
class AlphaZeroConfig:
    board_size: int = 14
    observation_channels: int = 49
    action_size: int = 21 * 8 * 14 * 14 + 1

    hidden_channels: int = 32
    residual_blocks: int = 2
    simulations: int = 32
    c_puct: float = 1.5
    dirichlet_alpha: float = 0.3
    dirichlet_epsilon: float = 0.25
    temperature_moves: int = 10

    self_play_games: int = 2
    max_game_length: int = 60
    replay_capacity: int = 2_000
    batch_size: int = 32
    training_epochs: int = 1
    learning_rate: float = 1e-3
    weight_decay: float = 1e-4
    seed: int = 20260911

    checkpoint_path: str = "checkpoints/mvp_latest.pt"

    # Iterative training defaults.  The original one-shot MVP continues to use
    # `self_play_games` and `checkpoint_path` above.
    training_iterations: int = 10
    iteration_self_play_games: int = 10
    iteration_simulations: int = 64
    iteration_training_epochs: int = 3
    arena_games: int = 20
    promotion_threshold: float = 0.55
    best_checkpoint_path: str = "checkpoints/best.pt"
    iteration_checkpoint_dir: str = "checkpoints/iterations"
    replay_path: str = "self_play_data/replay_buffer.npz"
    training_state_path: str = "logs/training_state.json"
    iteration_log_path: str = "logs/training_iterations.jsonl"
