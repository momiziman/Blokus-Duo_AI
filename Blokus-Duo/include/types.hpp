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
  static constexpr std::uint16_t PASS_CODE = 0xffff;
  static constexpr std::uint16_t INVALID_CODE = 0xfffe;

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

  bool is_valid() const {
    if (is_pass())
      return true;
    if (block_id.size() != 1)
      return false;
    int block_index = block_id[0] - 'a';
    return 0 <= block_index && block_index < 21 && 0 <= x && x < 16 &&
           0 <= y && y < 16 && 0 <= rotation && rotation < 8;
  }

  std::uint16_t packed() const {
    if (is_pass())
      return PASS_CODE;
    if (!is_valid())
      return INVALID_CODE;

    std::uint16_t block_index =
        static_cast<std::uint16_t>(block_id[0] - 'a');
    return static_cast<std::uint16_t>((block_index << 11) |
                                      ((rotation & 0x7) << 8) |
                                      ((x & 0xf) << 4) | (y & 0xf));
  }

  static Move from_packed(std::uint16_t code) {
    if (code == PASS_CODE)
      return Move();
    if (code == INVALID_CODE)
      return Move("?", -1, -1, 0);

    int y = code & 0xf;
    int x = (code >> 4) & 0xf;
    int rotation = (code >> 8) & 0x7;
    int block_index = (code >> 11) & 0x1f;
    if (block_index < 0 || block_index >= 21)
      return Move("?", -1, -1, 0);

    return Move(std::string(1, static_cast<char>('a' + block_index)), x, y,
                rotation);
  }
};
