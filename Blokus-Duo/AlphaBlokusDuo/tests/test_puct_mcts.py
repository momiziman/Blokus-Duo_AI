import unittest

import numpy as np
import torch

from alphablokus.config import AlphaZeroConfig
from alphablokus.game import AlphaGameState
from alphablokus.network import PolicyValueNetwork
from alphablokus.puct_mcts import PUCTSearch


class PUCTTest(unittest.TestCase):
    def test_search_returns_legal_policy(self) -> None:
        config = AlphaZeroConfig(hidden_channels=8, residual_blocks=1, simulations=3)
        model = PolicyValueNetwork(config)
        state = AlphaGameState()
        search = PUCTSearch(model, config, "cpu", np.random.default_rng(1))
        root = search.run(state)
        policy = search.policy(root, temperature=1.0)
        self.assertEqual(root.visit_count, config.simulations)
        self.assertAlmostEqual(float(policy.sum()), 1.0, places=6)
        selected = set(np.flatnonzero(policy))
        self.assertTrue(selected.issubset(set(state.legal_actions())))


if __name__ == "__main__":
    unittest.main()
