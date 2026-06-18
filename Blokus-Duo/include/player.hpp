#pragma once

#include "types.hpp"

struct Player {
  Color color;
  vector<string> used_blocks;
  int score = 0;
  int turn_num;
};
