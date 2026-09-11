from __future__ import annotations

from pathlib import Path

import numpy as np

from .replay_buffer import ReplayBuffer, TrainingExample


SCHEMA_VERSION = 1


def save_replay_buffer(path: str | Path, replay: ReplayBuffer) -> None:
    """Persist a replay buffer with sparse policies and an atomic replace."""
    destination = Path(path)
    destination.parent.mkdir(parents=True, exist_ok=True)
    temporary = destination.with_suffix(destination.suffix + ".tmp")
    examples = replay.to_list()

    if examples:
        observations = np.stack(
            [example.observation for example in examples]
        ).astype(np.float32, copy=False)
        values = np.asarray([example.value for example in examples], dtype=np.float32)
        action_size = int(examples[0].policy.size)
        policy_offsets = [0]
        policy_indices_parts: list[np.ndarray] = []
        policy_values_parts: list[np.ndarray] = []
        for example in examples:
            if example.policy.size != action_size:
                raise ValueError("all replay policies must have the same action size")
            indices = np.flatnonzero(example.policy).astype(np.int32, copy=False)
            policy_indices_parts.append(indices)
            policy_values_parts.append(example.policy[indices].astype(np.float32, copy=False))
            policy_offsets.append(policy_offsets[-1] + len(indices))
        policy_indices = np.concatenate(policy_indices_parts)
        policy_values = np.concatenate(policy_values_parts)
    else:
        observations = np.empty((0,), dtype=np.float32)
        values = np.empty((0,), dtype=np.float32)
        action_size = 0
        policy_offsets = [0]
        policy_indices = np.empty((0,), dtype=np.int32)
        policy_values = np.empty((0,), dtype=np.float32)

    with temporary.open("wb") as output:
        np.savez_compressed(
            output,
            schema_version=np.asarray([SCHEMA_VERSION], dtype=np.int32),
            observations=observations,
            values=values,
            action_size=np.asarray([action_size], dtype=np.int32),
            policy_offsets=np.asarray(policy_offsets, dtype=np.int64),
            policy_indices=policy_indices,
            policy_values=policy_values,
        )
    temporary.replace(destination)


def load_replay_buffer(path: str | Path, capacity: int) -> ReplayBuffer:
    source = Path(path)
    replay = ReplayBuffer(capacity)
    if not source.exists():
        return replay

    with np.load(source, allow_pickle=False) as data:
        schema_version = int(data["schema_version"][0])
        if schema_version != SCHEMA_VERSION:
            raise ValueError(
                f"unsupported replay schema {schema_version}; expected {SCHEMA_VERSION}"
            )
        observations = np.asarray(data["observations"], dtype=np.float32)
        values = np.asarray(data["values"], dtype=np.float32)
        action_size = int(data["action_size"][0])
        offsets = np.asarray(data["policy_offsets"], dtype=np.int64)
        indices = np.asarray(data["policy_indices"], dtype=np.int64)
        policy_values = np.asarray(data["policy_values"], dtype=np.float32)

        count = len(values)
        if len(observations) != count or len(offsets) != count + 1:
            raise ValueError("replay file contains inconsistent example counts")
        if offsets[0] != 0 or offsets[-1] != len(indices):
            raise ValueError("replay file contains invalid policy offsets")
        if len(indices) != len(policy_values):
            raise ValueError("replay file contains inconsistent sparse policies")

        start_example = max(0, count - capacity)
        examples: list[TrainingExample] = []
        for index in range(start_example, count):
            begin = int(offsets[index])
            end = int(offsets[index + 1])
            policy = np.zeros(action_size, dtype=np.float32)
            policy[indices[begin:end]] = policy_values[begin:end]
            examples.append(
                TrainingExample(
                    observation=np.array(observations[index], copy=True),
                    policy=policy,
                    value=float(values[index]),
                )
            )
    replay.add_examples(examples)
    return replay
