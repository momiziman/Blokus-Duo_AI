#pragma once

#include "types.hpp"

struct Player {
  Color color;
  vector<string> used_blocks;
  uint32_t used_mask = 0;
  int score = 0;
  int turn_num;
};

inline int block_index_from_id(const std::string &block_id) {
  if (block_id.empty())
    return -1;
  return block_id[0] - 'a';
}

inline bool is_block_used(const Player &player, const std::string &block_id) {
  int index = block_index_from_id(block_id);
  if (index < 0 || index >= 21)
    return false;
  return (player.used_mask & (1u << index)) != 0;
}

inline void mark_block_used(Player &player, const std::string &block_id) {
  int index = block_index_from_id(block_id);
  if (0 <= index && index < 21)
    player.used_mask |= (1u << index);
}
