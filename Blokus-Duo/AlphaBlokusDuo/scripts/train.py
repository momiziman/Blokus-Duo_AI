from __future__ import annotations

import json
import random
from pathlib import Path

import numpy as np
import torch

from alphablokus.checkpoint import save_checkpoint
from alphablokus.config import AlphaZeroConfig
from alphablokus.game import AlphaGameState
from alphablokus.network import PolicyValueNetwork
from alphablokus.replay_buffer import ReplayBuffer
from alphablokus.self_play import SelfPlayRunner
from alphablokus.trainer import Trainer


def main() -> None:
    config = AlphaZeroConfig()
    random.seed(config.seed)
    np.random.seed(config.seed)
    torch.manual_seed(config.seed)
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"device={device} simulations={config.simulations}")

    model = PolicyValueNetwork(config).to(device)
    replay = ReplayBuffer(config.replay_capacity)
    runner = SelfPlayRunner(model, config, device, AlphaGameState)

    game_summaries = []
    for game_index in range(config.self_play_games):
        examples, scores = runner.play_game()
        replay.add_game(examples)
        game_summaries.append({"game": game_index + 1, "scores": scores})
        print(
            f"self_play={game_index + 1}/{config.self_play_games} "
            f"positions={len(examples)} scores={scores}"
        )

    trainer = Trainer(model, config, device)
    losses = trainer.train(replay)
    checkpoint = Path(config.checkpoint_path)
    save_checkpoint(checkpoint, model, trainer.optimizer, config, step=1)

    Path("logs").mkdir(exist_ok=True)
    Path("logs/mvp_training.json").write_text(
        json.dumps(
            {"device": str(device), "games": game_summaries, "losses": losses},
            indent=2,
        ),
        encoding="utf-8",
    )
    print(f"losses={losses}")
    print(f"checkpoint={checkpoint}")


if __name__ == "__main__":
    main()
