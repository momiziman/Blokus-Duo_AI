import unittest

from alphablokus.action_encoder import (
    ACTION_SIZE,
    PASS_ACTION,
    action_to_components,
    components_to_action,
)


class ActionEncoderTest(unittest.TestCase):
    def test_round_trip(self) -> None:
        cases = [(0, 0, 1, 1), (20, 7, 14, 14), (9, 3, 6, 10)]
        for expected in cases:
            with self.subTest(expected=expected):
                action = components_to_action(*expected)
                self.assertEqual(action_to_components(action), expected)

    def test_pass_is_last_action(self) -> None:
        self.assertEqual(PASS_ACTION, ACTION_SIZE - 1)
        self.assertIsNone(action_to_components(PASS_ACTION))


if __name__ == "__main__":
    unittest.main()
