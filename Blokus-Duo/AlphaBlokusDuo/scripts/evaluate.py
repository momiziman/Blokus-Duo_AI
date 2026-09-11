from __future__ import annotations

import argparse
from pathlib import Path

import torch

from alphablokus.arena import evaluate_against_baseline
from alphablokus.checkpoint import load_checkpoint
from alphablokus.config import AlphaZeroConfig
from alphablokus.game import AlphaGameState
from alphablokus.network import PolicyValueNetwork


def main() -> None:
    parser = argparse.ArgumentParser(description="Evaluate AlphaBlokusDuo")
    parser.add_argument(
        "--baseline", choices=("random", "standard", "eval"), default="random"
    )
    parser.add_argument("--games", type=int, default=4)
    parser.add_argument("--checkpoint", default="checkpoints/mvp_latest.pt")
    parser.add_argument("--baseline-iterations", type=int, default=100)
    args = parser.parse_args()
    if args.games <= 0:
        parser.error("--games must be positive")
    if args.baseline_iterations <= 0:
        parser.error("--baseline-iterations must be positive")

    config = AlphaZeroConfig()
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = PolicyValueNetwork(config).to(device)
    checkpoint = Path(args.checkpoint)
    if checkpoint.exists():
        step = load_checkpoint(checkpoint, model, map_location=device)
        print(f"loaded={checkpoint} step={step}")
    else:
        print("checkpoint not found; evaluating the untrained network")

    result = evaluate_against_baseline(
        model,
        config,
        device,
        AlphaGameState,
        baseline=args.baseline,
        games=args.games,
        baseline_iterations=args.baseline_iterations,
    )
    print(f"device={device} games={args.games} baseline={args.baseline}")
    print(f"AlphaZero wins={result.alpha_wins}")
    print(f"{args.baseline} wins={result.baseline_wins}")
    print(f"draws={result.draws}")
    print(f"AlphaZero total score={result.alpha_score}")
    print(f"{args.baseline} total score={result.baseline_score}")


if __name__ == "__main__":
    main()
