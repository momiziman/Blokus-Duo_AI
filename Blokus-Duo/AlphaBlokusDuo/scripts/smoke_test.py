from __future__ import annotations

import numpy as np
import torch

from alphablokus.config import AlphaZeroConfig
from alphablokus.game import ACTION_SIZE, OBSERVATION_CHANNELS, AlphaGameState
from alphablokus.network import PolicyValueNetwork
from alphablokus.puct_mcts import PUCTSearch
from alphablokus.replay_buffer import ReplayBuffer
from alphablokus.self_play import SelfPlayRunner
from alphablokus.trainer import Trainer


def main() -> None:
    torch.manual_seed(7)
    config = AlphaZeroConfig(
        hidden_channels=8,
        residual_blocks=1,
        simulations=2,
        self_play_games=1,
        batch_size=8,
        training_epochs=1,
    )
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"device={device}")

    state = AlphaGameState()
    legal = state.legal_actions()
    observation = state.observation()
    assert len(legal) > 0
    assert len(observation) == OBSERVATION_CHANNELS * 14 * 14
    assert ACTION_SIZE == config.action_size
    print(f"initial_legal_actions={len(legal)}")

    model = PolicyValueNetwork(config).to(device)
    with torch.inference_mode():
        tensor = torch.tensor(observation, dtype=torch.float32, device=device)
        logits, value = model(tensor.reshape(1, OBSERVATION_CHANNELS, 14, 14))
    assert logits.shape == (1, ACTION_SIZE)
    assert value.shape == (1,)
    print(f"network_shapes=policy{tuple(logits.shape)} value{tuple(value.shape)}")

    search = PUCTSearch(model, config, device, np.random.default_rng(7))
    root = search.run(state, add_noise=False)
    policy = search.policy(root, temperature=1.0)
    assert np.isclose(policy.sum(), 1.0)
    print(f"puct_root_visits={root.visit_count}")

    runner = SelfPlayRunner(
        model,
        config,
        device,
        AlphaGameState,
        rng=np.random.default_rng(7),
    )
    examples, scores = runner.play_game()
    replay = ReplayBuffer(config.replay_capacity)
    replay.add_game(examples)
    trainer = Trainer(model, config, device)
    losses = trainer.train(replay)
    print(f"self_play_positions={len(examples)} final_scores={scores}")
    print("losses=" + " ".join(f"{key}:{value:.4f}" for key, value in losses.items()))
    print("SMOKE TEST PASSED")


if __name__ == "__main__":
    main()
