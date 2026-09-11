import unittest

import torch

from alphablokus.config import AlphaZeroConfig
from alphablokus.network import PolicyValueNetwork


class NetworkTest(unittest.TestCase):
    def test_output_shapes(self) -> None:
        config = AlphaZeroConfig(hidden_channels=8, residual_blocks=1)
        model = PolicyValueNetwork(config)
        inputs = torch.zeros(2, config.observation_channels, 14, 14)
        policy, value = model(inputs)
        self.assertEqual(tuple(policy.shape), (2, config.action_size))
        self.assertEqual(tuple(value.shape), (2,))
        self.assertTrue(torch.all(value <= 1.0))
        self.assertTrue(torch.all(value >= -1.0))


if __name__ == "__main__":
    unittest.main()
