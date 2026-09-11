from __future__ import annotations

import math
from dataclasses import dataclass, field

import numpy as np
import torch

from .config import AlphaZeroConfig
from .state_encoder import encode_state


@dataclass
class SearchNode:
    prior: float
    to_play: int
    visit_count: int = 0
    value_sum: float = 0.0
    children: dict[int, "SearchNode"] = field(default_factory=dict)

    @property
    def value(self) -> float:
        return self.value_sum / self.visit_count if self.visit_count else 0.0


class PUCTSearch:
    def __init__(
        self,
        model: torch.nn.Module,
        config: AlphaZeroConfig,
        device: torch.device | str,
        rng: np.random.Generator | None = None,
    ) -> None:
        self.model = model
        self.config = config
        self.device = torch.device(device)
        self.rng = rng or np.random.default_rng(config.seed)

    @torch.inference_mode()
    def _evaluate(self, state) -> tuple[dict[int, float], float]:
        observation = encode_state(state, self.config, self.device).unsqueeze(0)
        self.model.eval()
        logits, value = self.model(observation)
        legal = state.legal_actions()
        if not legal:
            return {}, float(value.item())
        legal_tensor = torch.tensor(legal, dtype=torch.long, device=self.device)
        legal_logits = logits[0].index_select(0, legal_tensor)
        probabilities = torch.softmax(legal_logits, dim=0).cpu().numpy()
        return dict(zip(legal, probabilities.tolist(), strict=True)), float(value.item())

    def _expand(self, node: SearchNode, state) -> float:
        if state.is_terminal:
            return float(state.terminal_value(state.current_player))
        priors, value = self._evaluate(state)
        next_player = 1 - state.current_player
        node.children = {
            action: SearchNode(prior=prior, to_play=next_player)
            for action, prior in priors.items()
        }
        return value

    def _add_root_noise(self, root: SearchNode) -> None:
        if not root.children:
            return
        actions = list(root.children)
        noise = self.rng.dirichlet(
            np.full(len(actions), self.config.dirichlet_alpha, dtype=np.float64)
        )
        fraction = self.config.dirichlet_epsilon
        for action, sample in zip(actions, noise, strict=True):
            child = root.children[action]
            child.prior = (1.0 - fraction) * child.prior + fraction * float(sample)

    def _select_child(self, node: SearchNode) -> tuple[int, SearchNode]:
        parent_scale = math.sqrt(max(1, node.visit_count))

        def score(item: tuple[int, SearchNode]) -> float:
            _, child = item
            # child.value is from the child-to-play perspective, so negate it
            # when choosing an action for the parent player.
            q_value = -child.value
            exploration = (
                self.config.c_puct
                * child.prior
                * parent_scale
                / (1 + child.visit_count)
            )
            return q_value + exploration

        return max(node.children.items(), key=score)

    @staticmethod
    def _backup(path: list[SearchNode], leaf_value: float, leaf_player: int) -> None:
        for node in path:
            node.visit_count += 1
            node.value_sum += leaf_value if node.to_play == leaf_player else -leaf_value

    def run(self, root_state, add_noise: bool = False) -> SearchNode:
        root = SearchNode(prior=1.0, to_play=root_state.current_player)
        self._expand(root, root_state)
        if add_noise:
            self._add_root_noise(root)

        for _ in range(self.config.simulations):
            state = root_state.clone()
            node = root
            path = [node]

            while node.children and not state.is_terminal:
                action, node = self._select_child(node)
                state.play_action(action)
                path.append(node)

            leaf_player = state.current_player
            leaf_value = self._expand(node, state)
            self._backup(path, leaf_value, leaf_player)
        return root

    def policy(self, root: SearchNode, temperature: float) -> np.ndarray:
        policy = np.zeros(self.config.action_size, dtype=np.float32)
        if not root.children:
            return policy
        actions = np.fromiter(root.children.keys(), dtype=np.int64)
        visits = np.fromiter(
            (child.visit_count for child in root.children.values()), dtype=np.float64
        )
        if temperature <= 1e-8:
            policy[actions[int(np.argmax(visits))]] = 1.0
            return policy
        weights = np.power(visits, 1.0 / temperature)
        if weights.sum() == 0:
            weights = np.ones_like(weights)
        policy[actions] = (weights / weights.sum()).astype(np.float32)
        return policy

    def select_action(self, policy: np.ndarray, sample: bool) -> int:
        if sample:
            return int(self.rng.choice(len(policy), p=policy))
        return int(np.argmax(policy))
