import unittest

from alphablokus.action_encoder import action_to_components
from alphablokus.game import ACTION_SIZE, OBSERVATION_CHANNELS, AlphaGameState


class GameBridgeTest(unittest.TestCase):
    def test_initial_state(self) -> None:
        state = AlphaGameState()
        self.assertEqual(state.current_player, 0)
        self.assertEqual(tuple(state.scores), (0, 0))
        self.assertFalse(state.is_terminal)
        self.assertEqual(len(state.observation()), OBSERVATION_CHANNELS * 14 * 14)
        self.assertEqual(len(state.legal_actions()), 712)

    def test_cpp_and_python_action_encoders_agree(self) -> None:
        state = AlphaGameState()
        for action in state.legal_actions()[:25]:
            move = state.action_to_move(action)
            self.assertEqual(state.move_to_action(move), action)
            components = action_to_components(action)
            self.assertIsNotNone(components)
            piece, rotation, x, y = components
            self.assertEqual(move.block_id, chr(ord("a") + piece))
            self.assertEqual((move.rotation, move.x, move.y), (rotation, x, y))

    def test_random_game_reaches_terminal(self) -> None:
        state = AlphaGameState()
        for move_number in range(60):
            if state.is_terminal:
                break
            state.play(state.select_baseline("random", seed=100 + move_number))
        self.assertTrue(state.is_terminal)
        self.assertIn(state.terminal_value(0), (-1.0, 0.0, 1.0))

    def test_action_space_matches_bridge(self) -> None:
        self.assertEqual(ACTION_SIZE, 32_929)


if __name__ == "__main__":
    unittest.main()
