import tempfile
import unittest
from pathlib import Path

import numpy as np

from alphablokus.replay_buffer import ReplayBuffer, TrainingExample
from alphablokus.replay_storage import load_replay_buffer, save_replay_buffer


class ReplayStorageTest(unittest.TestCase):
    def test_sparse_round_trip_and_capacity(self) -> None:
        replay = ReplayBuffer(3)
        for index in range(3):
            policy = np.zeros(11, dtype=np.float32)
            policy[index] = 0.25
            policy[index + 4] = 0.75
            replay.add_examples(
                [
                    TrainingExample(
                        observation=np.full((2, 2), index, dtype=np.float32),
                        policy=policy,
                        value=float(index - 1),
                    )
                ]
            )

        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "replay.npz"
            save_replay_buffer(path, replay)
            restored = load_replay_buffer(path, capacity=2)

        self.assertEqual(len(restored), 2)
        examples = restored.to_list()
        np.testing.assert_array_equal(
            examples[0].observation, np.full((2, 2), 1, dtype=np.float32)
        )
        np.testing.assert_allclose(examples[1].policy, replay.to_list()[2].policy)
        self.assertEqual(examples[1].value, 1.0)


if __name__ == "__main__":
    unittest.main()
