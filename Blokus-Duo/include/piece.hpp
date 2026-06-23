#pragma once

#include "types.hpp"

struct BlockData {
  std::array<std::array<int, 5>, 5> shape;
  std::array<std::array<int, 7>, 7> influence;
  int score;
};

extern const std::unordered_map<std::string, BlockData> block_table;
extern std::vector<std::string> BLOCK_IDS_BY_SIZE;

struct Block {
  vector<vector<int>> shape;
  vector<vector<int>> influence;
  vector<Position> cells;

  Block(const BlockData &data);

  vector<vector<int>> transpose(const vector<vector<int>> &mat);
  vector<vector<int>> rot90(const vector<vector<int>> &mat, int k);
  void rebuild_occupied_offsets();
  const vector<Position> &occupied_offsets() const;
  void rotate_block(int dir);
};

BlockData getBlock(const std::string &id);
void init_block_ids_by_size();
