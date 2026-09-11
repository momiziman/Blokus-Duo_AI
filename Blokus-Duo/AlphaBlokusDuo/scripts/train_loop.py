from __future__ import annotations

import argparse
import random
from dataclasses import replace

import numpy as np
import torch

from alphablokus.config import AlphaZeroConfig
from alphablokus.game import AlphaGameState
from alphablokus.iteration import AlphaZeroLearningLoop


def select_device(name: str) -> torch.device:
    if name == "auto":
        return torch.device("cuda" if torch.cuda.is_available() else "cpu")
    if name == "cuda" and not torch.cuda.is_available():
        raise RuntimeError("CUDA was requested but is not available")
    return torch.device(name)


def main() -> None:
    defaults = AlphaZeroConfig()
    parser = argparse.ArgumentParser(
        description="Run the resumable AlphaBlokusDuo training loop"
    )
    parser.add_argument(
        "--iterations",
        type=int,
        default=defaults.training_iterations,
        help="total iteration number to reach (resume-aware)",
    )
    parser.add_argument(
        "--self-play-games",
        type=int,
        default=defaults.iteration_self_play_games,
    )
    parser.add_argument("--arena-games", type=int, default=defaults.arena_games)
    parser.add_argument(
        "--simulations", type=int, default=defaults.iteration_simulations
    )
    parser.add_argument(
        "--training-epochs",
        type=int,
        default=defaults.iteration_training_epochs,
    )
    parser.add_argument(
        "--promotion-threshold",
        type=float,
        default=defaults.promotion_threshold,
    )
    parser.add_argument(
        "--device", choices=("auto", "cpu", "cuda"), default="auto"
    )
    args = parser.parse_args()

    if args.iterations <= 0:
        parser.error("--iterations must be positive")
    if args.self_play_games <= 0:
        parser.error("--self-play-games must be positive")
    if args.arena_games <= 0 or args.arena_games % 2 != 0:
        parser.error("--arena-games must be a positive even number")
    if args.simulations <= 0:
        parser.error("--simulations must be positive")
    if args.training_epochs <= 0:
        parser.error("--training-epochs must be positive")
    if not 0.0 <= args.promotion_threshold <= 1.0:
        parser.error("--promotion-threshold must be between 0 and 1")

    config = replace(
        defaults,
        training_iterations=args.iterations,
        iteration_self_play_games=args.self_play_games,
        arena_games=args.arena_games,
        iteration_simulations=args.simulations,
        iteration_training_epochs=args.training_epochs,
        promotion_threshold=args.promotion_threshold,
    )
    random.seed(config.seed)
    np.random.seed(config.seed)
    torch.manual_seed(config.seed)
    device = select_device(args.device)
    print(
        f"device={device} target_iterations={config.training_iterations} "
        f"self_play_games={config.iteration_self_play_games} "
        f"simulations={config.iteration_simulations} "
        f"arena_games={config.arena_games}"
    )

    loop = AlphaZeroLearningLoop(config, device, AlphaGameState)
    results = loop.run()
    if results:
        promoted = sum(result.promoted for result in results)
        print(
            f"completed_now={len(results)} promoted_now={promoted} "
            f"last_iteration={results[-1].iteration}"
        )


if __name__ == "__main__":
    main()
