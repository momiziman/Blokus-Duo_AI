#pragma once

#include <bits/stdc++.h>

using namespace std;
using std::array, std::vector;
using std::cout, std::endl, std::cin;
using std::deque;
using std::pair;
using std::swap;
using std::vector;

constexpr int TILE_NUMBER = 14;
constexpr int COLOR_NUM = 2;
constexpr int BOARD_SIZE = TILE_NUMBER + 2;
constexpr double MAX_SCORE = 89.0;

enum class AIType { RANDOM, MCTS_WIN, MCTS_EVAL };

enum class GameResult { P1_WIN, P2_WIN, DRAW };

enum class GamePhase {
  OPENING,
  MIDDLE,
  ENDING
};

enum TileState {
  BLANK = 0,
  CANTSET = 1,
  ABLESET = 2,
  MYBLOCK = 3,
  OPBLOCK = 4
};

enum class Color { PLAYER1 = 0, PLAYER2 = 1 };

struct Position {
  int x;
  int y;
};

struct Move {
  std::string block_id;
  int x = -1;
  int y = -1;
  int rotation = 0;

  Move() = default;
  Move(std::string block_id, int x, int y, int rotation)
      : block_id(std::move(block_id)), x(x), y(y), rotation(rotation) {}

  bool is_pass() const {
    return block_id.empty();
  }
};
