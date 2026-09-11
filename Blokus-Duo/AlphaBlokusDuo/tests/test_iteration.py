import json
import tempfile
import unittest
from dataclasses import replace
from pathlib import Path

import torch

from alphablokus.config import AlphaZeroConfig
from alphablokus.game import AlphaGameState
from alphablokus.iteration import AlphaZeroLearningLoop


class IterationTest(unittest.TestCase):
    def test_one_iteration_and_resume(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            config = replace(
                AlphaZeroConfig(),
                hidden_channels=8,
                residual_blocks=1,
                training_iterations=1,
                iteration_self_play_games=1,
                iteration_simulations=1,
                iteration_training_epochs=1,
                arena_games=2,
                replay_capacity=100,
                batch_size=16,
                best_checkpoint_path=str(root / "checkpoints" / "best.pt"),
                iteration_checkpoint_dir=str(root / "checkpoints" / "iterations"),
                replay_path=str(root / "data" / "replay.npz"),
                training_state_path=str(root / "logs" / "state.json"),
                iteration_log_path=str(root / "logs" / "iterations.jsonl"),
            )
            loop = AlphaZeroLearningLoop(
                config, torch.device("cpu"), AlphaGameState, progress=lambda _: None
            )
            results = loop.run()

            self.assertEqual(len(results), 1)
            self.assertEqual(results[0].iteration, 1)
            self.assertTrue(Path(config.best_checkpoint_path).exists())
            self.assertTrue(Path(config.replay_path).exists())
            self.assertTrue(
                (Path(config.iteration_checkpoint_dir) / "iteration_0001.pt").exists()
            )
            state = json.loads(
                Path(config.training_state_path).read_text(encoding="utf-8")
            )
            self.assertEqual(state["completed_iterations"], 1)
            self.assertIsNone(state["in_progress"])

            resumed = AlphaZeroLearningLoop(
                config, torch.device("cpu"), AlphaGameState, progress=lambda _: None
            )
            self.assertEqual(resumed.run(), [])


if __name__ == "__main__":
    unittest.main()
