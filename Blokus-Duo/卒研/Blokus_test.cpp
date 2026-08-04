
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include "block_data.h"
#include <algorithm>
#include <array>
#include <bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <stdio.h>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
using std::array, std::vector;
using std::cout, std::endl, std::cin;
using std::deque;
using std::pair;
using std::swap;
using std::vector;
using namespace std;

const std::unordered_map<std::string, BlockData> block_table = {
    // shape (5x5)
    {"a",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      1}}, // score
    {"b",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      2}}, // score
    {"c",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      3}}, // score
    {"d",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      3}}, // score
    {"e",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0}}},
      4}}, // score
    {"f",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"g",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"h",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"i",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 2, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"j",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0}}},
      5}}, // score
    {"k",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"l",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 2, 0, 0},
        {0, 2, 1, 2, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"m",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"n",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"o",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0}}},
      5}}, // score
    {"p",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"q",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 2},
        {0, 0, 1, 1, 1, 1, 1},
        {0, 0, 2, 1, 1, 1, 2},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"r",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 2, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 1, 0},
        {0, 0, 0, 2, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"s",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 2, 0, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 1, 0},
        {0, 0, 0, 2, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"t",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 2, 0, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"u",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score

};

constexpr int TILE_NUMBER = 14;
constexpr int COLOR_NUM = 2;
constexpr int BOARD_SIZE = TILE_NUMBER + 2; // 壁を含めたサイズ

constexpr double MAX_SCORE = 89.0;

enum class AIType { RANDOM, MCTS_WIN, MCTS_EVAL };

enum class GameResult { P1_WIN, P2_WIN, DRAW };

enum class GamePhase {
  OPENING, // 序盤
  MIDDLE,  // 中盤
  ENDING   // 終盤
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

struct MCTSNode;                           // MCTSNode を前方宣言
void log_node_basic(const MCTSNode *node); // 関数の前方宣言

std::vector<std::string> BLOCK_IDS_BY_SIZE;

void init_block_ids_by_size() {
  BLOCK_IDS_BY_SIZE.clear();

  for (auto &[id, data] : block_table) {
    BLOCK_IDS_BY_SIZE.push_back(id);
  }

  std::sort(BLOCK_IDS_BY_SIZE.begin(), BLOCK_IDS_BY_SIZE.end(),
            [&](const std::string &a, const std::string &b) {
              return block_table.at(a).score > block_table.at(b).score;
            });
}

struct Block {
  vector<vector<int>> shape;
  vector<vector<int>> influence;

  Block(const BlockData &data) {
    // std::array → vector に変換
    shape.resize(5, vector<int>(5));
    for (int i = 0; i < 5; ++i)
      for (int j = 0; j < 5; ++j)
        shape[i][j] = data.shape[i][j];

    influence.resize(7, vector<int>(7));
    for (int i = 0; i < 7; ++i)
      for (int j = 0; j < 7; ++j)
        influence[i][j] = data.influence[i][j];
  }

  // 2次元ベクトルを転置
  vector<vector<int>> transpose(const vector<vector<int>> &mat) {
    size_t rows = mat.size();
    size_t cols = mat[0].size();
    vector<vector<int>> res(cols, vector<int>(rows));
    for (size_t i = 0; i < rows; ++i)
      for (size_t j = 0; j < cols; ++j)
        res[j][i] = mat[i][j];
    return res;
  }

  // 2次元ベクトルを90°回転（時計回りにk回）
  vector<vector<int>> rot90(const vector<vector<int>> &mat, int k) {
    k = ((k % 4) + 4) % 4; // 安全に0〜3に正規化
    vector<vector<int>> res = mat;
    for (int i = 0; i < k; ++i) {
      size_t rows = res.size();
      size_t cols = res[0].size();
      vector<vector<int>> tmp(cols, vector<int>(rows));
      for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
          tmp[c][rows - 1 - r] = res[r][c];
      res = tmp;
    }
    return res;
  }

  void rotate_block(int dir) {
    switch (dir) {
    case 0: // 初期向き
      break;

    case 1:                               // 裏向き
      shape = rot90(transpose(shape), 1); // Pythonでは -1
      influence = rot90(transpose(influence), 1);
      break;

    case 2: // 初期向きから90°時計回り
      shape = rot90(shape, 1);
      influence = rot90(influence, 1);
      break;

    case 3: // 裏向きから90°反時計回り
      shape = transpose(shape);
      influence = transpose(influence);
      break;

    case 4: // 初期向きから180°時計回り
      shape = rot90(shape, 2);
      influence = rot90(influence, 2);
      break;

    case 5: // 裏向きから180°反時計回り
      shape = rot90(transpose(shape), 3);
      influence = rot90(transpose(influence), 3);
      break;

    case 6: // 初期向きから270°時計回り
      shape = rot90(shape, 3);
      influence = rot90(influence, 3);
      break;

    case 7: // 裏向きから270°反時計回り
      shape = rot90(transpose(shape), 2);
      influence = rot90(transpose(influence), 2);
      break;
    }
  }
};

BlockData getBlock(const std::string &id) {
  auto it = block_table.find(id);
  if (it == block_table.end()) {
    throw std::runtime_error("Error: block id '" + id + "' not found!");
  }
  return it->second;
}

struct Player {
  Color color;
  vector<string> used_blocks; // 使用済みブロックIDを記録
  int score = 0;
  int turn_num;
};

class Board {
public:
  inline static const int BLANK = 0;
  inline static const int CANTSET = 1;
  inline static const int ABLESET = 2;
  inline static const int BLOCK = 3;
  inline static const int OPBLOCK = 4;

  int TILE_NUMBER;

  // status[color][y][x] の形で入力される
  vector<vector<vector<int>>>
      status; // [COLOR_NUM][TILE_NUMBER+2][TILE_NUMBER+2]

  Board(int tile_number, const vector<vector<vector<int>>> &input_board)
      : TILE_NUMBER(tile_number), status(input_board) {}

  // ---------- settable_check ----------
  bool settable_check(Color color, const vector<vector<int>> &block_shape,
                      int x, int y) {
    int col = static_cast<int>(color);
    bool found_corner = false;

    int H = block_shape.size();
    int W = block_shape[0].size();

    for (int i = 0; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        if (block_shape[i][j] != CANTSET)
          continue;

        int access_y = y + i - 2;
        int access_x = x + j - 2;

        if (access_y < 0 || access_y >= TILE_NUMBER + 2 || access_x < 0 ||
            access_x >= TILE_NUMBER + 2)
          continue;

        int cell = status[col][access_y][access_x];

        // 自分ブロック / OPBLOCK / CANTSET は置けない
        if (cell == MYBLOCK || cell == CANTSET || cell == OPBLOCK)
          return false;

        if (cell == ABLESET)
          found_corner = true; // 角接触
      }
    }

    return found_corner;
  }

  // ---------- 合法手探索 ----------
  vector<pair<int, int>>
  search_settable_position(Color color,
                           const vector<vector<int>> &block_shape) {
    vector<pair<int, int>> res;
    int col = static_cast<int>(color);

    for (int y = 1; y <= TILE_NUMBER; ++y) {
      for (int x = 1; x <= TILE_NUMBER; ++x) {
        if (status[col][y][x] != CANTSET) {
          if (settable_check(color, block_shape, x, y))
            res.push_back({x, y});
        }
      }
    }

    return res;
  }

  vector<pair<int, int>> search_settable_position_near_ableset(
      Color color, const vector<vector<int>> &block_shape) {
    vector<pair<int, int>> res;
    int col = static_cast<int>(color);

    std::set<pair<int, int>> candidates; // 重複防止

    // --- ABLESET を起点に候補座標を収集 ---
    for (int y = 1; y <= TILE_NUMBER; ++y) {
      for (int x = 1; x <= TILE_NUMBER; ++x) {

        if (status[col][y][x] != ABLESET)
          continue;

        // 周囲5マス（中心含む）
        for (int dy = -2; dy <= 2; ++dy) {
          for (int dx = -2; dx <= 2; ++dx) {
            int ny = y + dy;
            int nx = x + dx;

            if (1 <= ny && ny <= TILE_NUMBER && 1 <= nx && nx <= TILE_NUMBER) {
              candidates.insert({nx, ny});
            }
          }
        }
      }
    }

    // --- 実際に置けるかチェック ---
    for (auto &[x, y] : candidates) {
      if (status[col][y][x] != CANTSET) {
        if (settable_check(color, block_shape, x, y)) {
          res.push_back({x, y});
        }
      }
    }

    return res;
  }

  vector<pair<int, int>> search_settable_position_one_ableset(
      Color color, const vector<vector<int>> &block_shape, int ax, int ay) {

    vector<pair<int, int>> res;
    int col = static_cast<int>(color);

    // ABLESET でなければ即終了
    if (status[col][ay][ax] != ABLESET) {
      cout << "Error:It's not ABLESET! (by search settable position one "
              "ableset())"
           << endl;
      return res;
    }

    std::set<pair<int, int>> candidates; // 重複防止

    // --- 周囲候補生成（5×5） ---
    for (int dy = -2; dy <= 2; ++dy) {
      for (int dx = -2; dx <= 2; ++dx) {
        int ny = ay + dy;
        int nx = ax + dx;

        if (1 <= ny && ny <= TILE_NUMBER && 1 <= nx && nx <= TILE_NUMBER) {
          candidates.insert({nx, ny});
        }
      }
    }

    // --- 実際に置けるかチェック ---
    for (auto &[cx, cy] : candidates) {
      if (status[col][cy][cx] != CANTSET &&
          settable_check(color, block_shape, cx, cy)) {
        res.emplace_back(cx, cy);
      }
    }

    return res;
  }

  std::optional<std::pair<int, int>>
  select_random_settable_position(Color color) {
    vector<pair<int, int>> candidates;
    int col = static_cast<int>(color);

    // --- ABLESET を起点に候補座標を収集 ---
    for (int y = 1; y <= TILE_NUMBER; ++y) {
      for (int x = 1; x <= TILE_NUMBER; ++x) {
        if (status[col][y][x] == ABLESET) {
          candidates.emplace_back(x, y);
        }
      }
    }
    // 候補なし
    if (candidates.empty()) {
      cout << "No one ABLESET." << endl;
      return std::nullopt;
    }
    // --- ランダムに1つ選択 ---
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, candidates.size() - 1);
    return candidates[dist(gen)];
  }

  // Boardクラス内
  void change_status(Color color, Block &block, const std::string &block_id,
                     int rotation, int x, int y, Player &player) {
    block.rotate_block(rotation);

    int col = static_cast<int>(color);
    int opp_col = 1 - col; // 相手の色
    int H = status[0].size();
    int W = status[0][0].size();

    auto in_bounds = [&](int yy, int xx) {
      return (0 <= yy && yy < H && 0 <= xx && xx < W);
    };

    // --- 自分の盤面に shape を MYBLOCK として反映 ---
    int S = block.shape.size();
    for (int r = 0; r < S; r++) {
      for (int c = 0; c < S; c++) {
        if (block.shape[r][c] != CANTSET)
          continue;

        int yy = y + r - 2;
        int xx = x + c - 2;
        if (!in_bounds(yy, xx))
          continue;

        status[col][yy][xx] = MYBLOCK;
      }
    }

    // --- 自分の盤面に influence を適用（ABLESET / CANTSET） ---
    int I = block.influence.size();
    for (int r = 0; r < I; r++) {
      for (int c = 0; c < I; c++) {
        int yy = y + r - 3;
        int xx = x + c - 3;
        if (!in_bounds(yy, xx))
          continue;

        // 既に MYBLOCK / OPBLOCK がある場合は上書きしない
        if (status[col][yy][xx] == MYBLOCK || status[col][yy][xx] == OPBLOCK)
          continue;

        if (block.influence[r][c] == CANTSET)
          status[col][yy][xx] = CANTSET;
        else if (block.influence[r][c] == ABLESET &&
                 status[col][yy][xx] == BLANK)
          status[col][yy][xx] = ABLESET;
      }
    }

    // --- 相手の盤面に MYBLOCK を OPBLOCK として反映 ---
    for (int r = 0; r < S; r++) {
      for (int c = 0; c < S; c++) {
        if (block.shape[r][c] != CANTSET)
          continue;

        int yy = y + r - 2;
        int xx = x + c - 2;
        if (!in_bounds(yy, xx))
          continue;

        // 相手盤面でも既に OPBLOCK があれば上書きしない
        if (status[opp_col][yy][xx] != OPBLOCK)
          status[opp_col][yy][xx] = OPBLOCK;
      }
    }

    // --- used_blocks に追加 ---
    player.used_blocks.push_back(block_id);
    player.turn_num++;

    // --- スコア加算 ---
    auto it = block_table.find(block_id);
    if (it != block_table.end())
      player.score += it->second.score;
  }

  void print_status(Color color) {
    int col = static_cast<int>(color);

    for (int y = 0; y < TILE_NUMBER + 2; y++) {
      for (int x = 0; x < TILE_NUMBER + 2; x++) {
        int cell = status[col][y][x];
        switch (cell) {
        case 0: // BLANK
          cout << ". ";
          break;
        case 1: // CANTSET
          cout << "# ";
          break;
        case 2: // ABLESET
          cout << "* ";
          break;
        case 3: // MYBLOCK
          cout << "\033[32m"
               << "3 "
               << "\033[0m"; // 緑
          break;
        case 4: // OPBLOCK
          cout << "\033[31m"
               << "4 "
               << "\033[0m"; // 赤
          break;
        default:
          cout << cell << " ";
          break;
        }
      }
      cout << endl;
    }
  }

  GamePhase get_phase(const Player &p1, const Player &p2) const {
    int total_score = p1.score + p2.score;

    if (total_score <= 39) {
      return GamePhase::OPENING;
    } else if (total_score <= 99) {
      return GamePhase::MIDDLE;
    } else {
      return GamePhase::ENDING;
    }
  }
};

int score_limitter(Player p) {
  int lim;
  if (p.turn_num <= 12) {
    lim = p.turn_num * 5;
  } else if (p.turn_num <= 17) {
    lim = 60 + (p.turn_num - 12) * 4;
  } else if (p.turn_num <= 19) {
    lim = 80 + (p.turn_num - 17) * 3;
  } else if (p.turn_num == 20) {
    lim = 88;
  } else if (p.turn_num == 21) {
    lim = 89;
  }
  return lim;
}

// Player クラスを引数に取って合法手リストを返す関数
vector<tuple<string, int, int, int>>
get_all_legal_moves(Board &board, Color player_color, Player &player) {
  vector<tuple<string, int, int, int>> legal_moves;

  // 使用済みブロックを除いた未使用ブロックリストを作成
  vector<string> unused_blocks;
  for (auto &[id, _] : block_table) {
    if (find(player.used_blocks.begin(), player.used_blocks.end(), id) ==
        player.used_blocks.end()) {
      unused_blocks.push_back(id);
    }
  }

  // 未使用ブロックで合法手探索
  for (auto &block_id : unused_blocks) {
    BlockData data = getBlock(block_id);
    Block block(data);

    for (int rot = 0; rot < 8; ++rot) {
      Block tmp_block = block;
      tmp_block.rotate_block(rot);

      auto positions = board.search_settable_position_near_ableset(
          player_color, tmp_block.shape);
      // board.search_settable_position_near_ableset(player_color,tmp_block.shape);

      for (auto &[x, y] : positions) {
        legal_moves.emplace_back(block_id, x, y, rot);
      }
    }
  }

  return legal_moves;
}

vector<tuple<string, int, int, int>>
get_fast_legal_moves(Board &board, Color color, Player &player, int max_moves) {

  vector<tuple<string, int, int, int>> moves;

  // --- 未使用ブロック列挙 ---
  vector<string> unused_blocks;
  for (auto &[id, _] : block_table) {
    if (find(player.used_blocks.begin(), player.used_blocks.end(), id) ==
        player.used_blocks.end()) {
      unused_blocks.push_back(id);
    }
  }

  if (unused_blocks.empty())
    return moves;

  static thread_local mt19937 gen(random_device{}());

  // 未使用ブロックの探索順をランダム化
  shuffle(unused_blocks.begin(), unused_blocks.end(), gen);

  string block_id = unused_blocks[0]; // 最初のブロックのみ使用

  BlockData data = getBlock(block_id);
  Block base(data);

  // --- 回転・反転 ---
  for (int rot = 0; rot < 8; ++rot) {
    Block tmp = base;
    tmp.rotate_block(rot);

    auto positions =
        board.search_settable_position_near_ableset(color, tmp.shape);

    for (auto &[x, y] : positions) {
      moves.emplace_back(block_id, x, y, rot);

      if ((int)moves.size() >= max_moves) {
        shuffle(moves.begin(), moves.end(), gen);
        return moves;
      }
    }
  }

  // --- 合法手が複数ある場合はランダム化 ---
  shuffle(moves.begin(), moves.end(), gen);

  return moves;
}

vector<tuple<string, int, int, int>>
get_one_legal_moves(Board &board, Color color, Player &player, int max_moves) {

  vector<tuple<string, int, int, int>> moves;

  // --- 未使用ブロック列挙 ---
  vector<string> unused_blocks;
  for (auto &[id, _] : block_table) {
    if (find(player.used_blocks.begin(), player.used_blocks.end(), id) ==
        player.used_blocks.end()) {
      unused_blocks.push_back(id);
    }
  }

  if (unused_blocks.empty())
    return moves;

  static thread_local mt19937 gen(random_device{}());

  // 未使用ブロックの探索順をランダム化
  shuffle(unused_blocks.begin(), unused_blocks.end(), gen);
  // --- 全未使用ブロックに対して探索 ---
  for (const auto &block_id : unused_blocks) {

    BlockData data = getBlock(block_id);
    Block base(data);

    // --- 回転・反転 ---
    for (int rot = 0; rot < 8; ++rot) {
      Block tmp = base;
      tmp.rotate_block(rot);

      auto positions =
          board.search_settable_position_near_ableset(color, tmp.shape);

      for (auto &[x, y] : positions) {
        moves.emplace_back(block_id, x, y, rot);

        if ((int)moves.size() >= max_moves) {
          shuffle(moves.begin(), moves.end(), gen);
          return moves;
        }
      }
    }
  }

  // --- 合法手が複数ある場合はランダム化 ---
  shuffle(moves.begin(), moves.end(), gen);

  return moves;
}

vector<tuple<string, int, int, int>>
get_oneable_legal_moves(Board &board, Color color, Player &player) {

  vector<tuple<string, int, int, int>> moves;

  // --- 未使用ブロック列挙 ---
  vector<string> unused_blocks;
  for (auto &[id, _] : block_table) {
    if (find(player.used_blocks.begin(), player.used_blocks.end(), id) ==
        player.used_blocks.end()) {
      unused_blocks.push_back(id);
    }
  }

  if (unused_blocks.empty())
    return moves;

  static thread_local mt19937 gen(random_device{}());

  // 未使用ブロックの探索順をランダム化
  shuffle(unused_blocks.begin(), unused_blocks.end(), gen);

  string block_id = unused_blocks[0]; // 最初のブロックのみ使用
  BlockData data = getBlock(block_id);
  Block base(data);
  int ax = -1;
  int ay = -1;
  // ① ABLESET をランダムに1つ選ぶ
  auto a = board.select_random_settable_position(color);
  if (a != nullopt) {
    auto [ax, ay] = a.value();
  }

  if (ax > 0 || ay > 0) {
    // --- 回転・反転 ---
    for (int rot = 0; rot < 8; ++rot) {
      Block tmp = base;
      tmp.rotate_block(rot);

      auto positions =
          board.search_settable_position_one_ableset(color, tmp.shape, ax, ay);

      for (auto &[x, y] : positions) {
        moves.emplace_back(block_id, x, y, rot);
      }
    }
  }
  return moves;
}

// Player クラスの used_blocks を考慮して合法手リストを取得（x,yを除く）
vector<pair<string, int>>
get_legal_moves_no_pos(Board &board, Color player_color, Player &player) {
  vector<pair<string, int>> legal_moves;

  for (auto &[id, _] : block_table) {
    // 使用済みブロックはスキップ
    if (find(player.used_blocks.begin(), player.used_blocks.end(), id) !=
        player.used_blocks.end())
      continue;

    BlockData data = getBlock(id);
    Block block(data);

    for (int rot = 0; rot < 8; ++rot) {
      Block tmp_block = block;
      tmp_block.rotate_block(rot);

      // 置ける場所が1つでもあれば合法手とみなす
      auto positions =
          board.search_settable_position(player_color, tmp_block.shape);
      if (!positions.empty()) {
        legal_moves.emplace_back(id, rot);
      }
    }
  }

  return legal_moves;
}

vector<string> get_legal_block_types(Board &board, Color player_color,
                                     Player &player) {
  vector<string> legal_blocks;

  for (auto &[id, _] : block_table) {
    // 使用済みブロックはスキップ
    if (find(player.used_blocks.begin(), player.used_blocks.end(), id) !=
        player.used_blocks.end())
      continue;

    BlockData data = getBlock(id);
    Block block(data);

    bool can_place = false;

    // 8方向の回転をチェック
    for (int rot = 0; rot < 8; ++rot) {
      Block tmp_block = block;
      tmp_block.rotate_block(rot);

      auto positions =
          board.search_settable_position(player_color, tmp_block.shape);
      if (!positions.empty()) {
        can_place = true;
        break; // 1つでも置ければ十分
      }
    }

    if (can_place)
      legal_blocks.push_back(id);
  }

  return legal_blocks;
}

int mobility(Board &board, Color color, Player &player) {
  int mob = get_all_legal_moves(board, color, player).size();
  return mob;
}

double evaluate(Board &board, Player &p1, Player &p2, Color turn,
                GamePhase phase) {
  double r = 0.0;
  int cant = 0;
  double w_score;
  double w_mymob;
  double w_opmob;
  double w_cant;

  int mymob = 0;
  int opmob = 0;

  switch (phase) {
  case GamePhase::OPENING:
    w_score = 1.0;
    w_mymob = 0.0;
    w_opmob = 0.0;
    w_cant = 0.0;
    break;
  case GamePhase::MIDDLE:
    w_score = 1.0;
    w_mymob = 0.0;
    w_opmob = 0.0;
    w_cant = 0.0;
    break;
  case GamePhase::ENDING:
    w_score = 1.0;
    w_mymob = 0.0;
    w_opmob = 0.0;
    w_cant = 0.0;
    break;
  }

  if (turn == Color::PLAYER1) {
    int score_lim = score_limitter(p1);
    int col = static_cast<int>(turn);
    if (w_mymob != 0) {
      mymob = mobility(board, Color::PLAYER1, p1);
    }
    if (w_opmob != 0) {
      opmob = mobility(board, Color::PLAYER2, p2);
    }
    for (int y = 1; y < TILE_NUMBER + 2; y++) {
      for (int x = 1; x < TILE_NUMBER + 2; x++) {
        int cell = board.status[col][y][x];
        if (cell == CANTSET)
          cant++;
      }
    }
    r = (w_score * p1.score / score_lim) - (w_cant * cant / 194) +
        (w_mymob * mymob / 2000) - (w_opmob * opmob / 2000);
  } else {
    int score_lim = score_limitter(p2);
    int col = static_cast<int>(Color::PLAYER2);
    if (w_mymob != 0) {
      mymob = mobility(board, Color::PLAYER2, p2);
    }
    if (w_opmob != 0) {
      opmob = mobility(board, Color::PLAYER1, p1);
    }
    for (int y = 1; y < TILE_NUMBER + 2; y++) {
      for (int x = 1; x < TILE_NUMBER + 2; x++) {
        int cell = board.status[col][y][x];
        if (cell == CANTSET)
          cant++;
      }
    }
    r = (w_score * p2.score / score_lim) - (w_cant * cant / 194) +
        (w_mymob * mymob / 2000) - (w_opmob * opmob / 2000);
  }
  return r;
}

pair<int, int> random_playout(Board board, Player player1, Player player2,
                              Color turn) {
  // cout << "=== Random Playout Start ===" << endl;
  // board.print_status(Color::PLAYER1); /*  デバッグ用  */

  Color current_color = turn;

  static thread_local std::mt19937 gen((std::random_device())());

  while (true) {
    Player *current_player =
        (current_color == Color::PLAYER1) ? &player1 : &player2;

    auto legal_moves =
        get_oneable_legal_moves(board, current_color, *current_player);

    if (legal_moves.empty()) {
      auto my = get_one_legal_moves(board, Color::PLAYER1, player1, 1);
      auto op = get_one_legal_moves(board, Color::PLAYER2, player2, 1);
      if (my.empty() && op.empty()) {
        break;
      } else {
        legal_moves =
            get_one_legal_moves(board, current_color, *current_player, 100);
        if (legal_moves.empty()) {
          current_color = (current_color == Color::PLAYER1) ? Color::PLAYER2
                                                            : Color::PLAYER1;
          continue;
        } else {
          std::uniform_int_distribution<> dis(0, (int)legal_moves.size() - 1);
          int idx = dis(gen);

          auto [block_id, x, y, rot] = legal_moves[idx];
          BlockData data = getBlock(block_id);
          Block block(data);

          board.change_status(current_color, block, block_id, rot, x, y,
                              *current_player);
          // board.print_status(current_color); /*  デバッグ用  */
        }
      }
    } else {
      std::uniform_int_distribution<> dis(0, (int)legal_moves.size() - 1);
      int idx = dis(gen);

      auto [block_id, x, y, rot] = legal_moves[idx];
      BlockData data = getBlock(block_id);
      Block block(data);

      /*cout << "Block id: " << block_id
           << ", legal_moves size: " << legal_moves.size() << endl;/* デバッグ用
       */
      board.change_status(current_color, block, block_id, rot, x, y,
                          *current_player);
      // board.print_status(current_color); /*  デバッグ用  */
    }

    current_color =
        (current_color == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;
  }

  return {player1.score, player2.score};
}

double heuristic_playout(Board board, Player p1, Player p2, Color turn) {
  int PLAYOUT_DEPTH;
  auto phase = board.get_phase(p1, p2);

  switch (phase) {
  case GamePhase::OPENING:
    PLAYOUT_DEPTH = 5;
    break;
  case GamePhase::MIDDLE:
    PLAYOUT_DEPTH = 7;
    break;
  case GamePhase::ENDING:
    PLAYOUT_DEPTH = 30;
    break;
  }

  for (int depth = 0; depth < PLAYOUT_DEPTH; depth++) {

    Player *cur = (turn == Color::PLAYER1) ? &p1 : &p2;

    auto moves = get_oneable_legal_moves(board, turn, *cur);
    if (moves.empty()) {
      auto my = get_one_legal_moves(board, Color::PLAYER1, p1, 1);
      auto op = get_one_legal_moves(board, Color::PLAYER2, p2, 1);
      if (my.empty() && op.empty()) {
        break;
      } else {
        moves = get_one_legal_moves(board, turn, *cur, 100);
        if (moves.empty()) {
          turn = (turn == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;
          continue;
        } else {
          static thread_local std::mt19937 gen((std::random_device())());
          std::uniform_int_distribution<> dis(0, (int)moves.size() - 1);
          int idx = dis(gen);

          auto [block_id, x, y, rot] = moves[idx];
          BlockData data = getBlock(block_id);
          Block block(data);
          board.change_status(turn, block, block_id, rot, x, y, *cur);
          /*cout << "Playout Step " << depth + 1 << ": "
                 << ((turn == Color::PLAYER1) ? "P1" : "P2") << " plays block "
                 << block_id << " at (" << x << "," << y << ") rot=" << rot <<
            "\n"; board.print_status(turn); /*  デバッグ用  */

          turn = (turn == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;
        }
      }
    }

    else {
      static thread_local std::mt19937 gen((std::random_device())());
      std::uniform_int_distribution<> dis(0, (int)moves.size() - 1);
      int idx = dis(gen);

      auto [block_id, x, y, rot] = moves[idx];
      BlockData data = getBlock(block_id);
      Block block(data);
      board.change_status(turn, block, block_id, rot, x, y, *cur);
      /*cout << "Playout Step " << depth + 1 << ": "
             << ((turn == Color::PLAYER1) ? "P1" : "P2") << " plays block "
             << block_id << " at (" << x << "," << y << ") rot=" << rot << "\n";
        board.print_status(turn); /*  デバッグ用  */

      turn = (turn == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;
    }
  }

  return evaluate(board, p1, p2, turn, phase);
}

struct MCTSNode {
  Board board;
  Player player1;
  Player player2;
  Color current_player;
  double eval_value = 0.0;

  MCTSNode *parent;
  std::vector<MCTSNode *> children;

  int visit_count;
  double win_score;

  int depth;

  std::vector<std::tuple<std::string, int, int, int>> untried_moves;

  // このノードへの着手
  std::string move_block_id;
  int move_x, move_y, move_rot;

  MCTSNode(const Board &b, const Player &p1, const Player &p2, Color turn,
           MCTSNode *parent = nullptr)
      : board(b), player1(p1), player2(p2), current_player(turn),
        parent(parent), visit_count(0), win_score(0.0), move_x(-1), move_y(-1),
        move_rot(0), move_block_id("") {
    depth = (parent == nullptr) ? 0 : parent->depth + 1;
  }

  // --- Selection: UCB1 で子ノード選択 ---
  MCTSNode *select_child() {
    const double C = 1.41421356;
    double best_value = -1e18;
    MCTSNode *best = nullptr;

    for (auto child : children) {
      // 未訪問ノードは優先的に選ぶ
      if (child->visit_count == 0) {
        return child;
      }
    }
    for (auto child : children) {
      double ucb = (child->win_score / (child->visit_count + 1e-6)) +
                   C * std::sqrt(std::log(visit_count + 1) /
                                 (child->visit_count + 1e-6));

      if (ucb > best_value) {
        best_value = ucb;
        best = child;
      }
    }
    return best;
  }

  // --- Expansion: 未展開手から子ノードを生成 ---
  MCTSNode *expand_node() {
    if (untried_moves.empty()) {
      return this;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, (int)untried_moves.size() - 1);

    int idx = dis(gen);
    auto [block_id, x, y, rot] = untried_moves[idx];

    // 選んだ手を未展開リストから削除
    untried_moves.erase(untried_moves.begin() + idx);

    // 次ノード用の盤面/プレイヤー状態をコピー
    Board next_board = board;
    Player next_p1 = player1;
    Player next_p2 = player2;

    Player *cur = (current_player == Color::PLAYER1) ? &next_p1 : &next_p2;

    // 盤面更新（next_board に対して）
    BlockData data = getBlock(block_id);
    Block blk(data);
    next_board.change_status(current_player, blk, block_id, rot, x, y, *cur);

    // 次ターンを決定
    Color next_turn =
        (current_player == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;

    // 子ノードを生成（parent=this）
    MCTSNode *child =
        new MCTSNode(next_board, next_p1, next_p2, next_turn, this);

    // 子ノードの着手情報を設定
    child->move_block_id = block_id;
    child->move_x = x;
    child->move_y = y;
    child->move_rot = rot;

    // 子を追加
    children.push_back(child);

    // 子ノードの未展開手（合法手）を計算して設定する
    Player *next_player =
        (next_turn == Color::PLAYER1) ? &child->player1 : &child->player2;
    child->untried_moves =
        get_all_legal_moves(child->board, next_turn, *next_player);

    return child;
  }

  // --- Simulation: playout の呼び出し ---
  double simulate(AIType ai_type) {
    Board sim_board = board;
    Player sim_p1 = player1;
    Player sim_p2 = player2;
    Color sim_turn = current_player;
    double result = -1.0;

    if (ai_type == AIType::MCTS_WIN) {
      auto [score1, score2] =
          random_playout(sim_board, sim_p1, sim_p2, sim_turn);

      if (current_player == Color::PLAYER1) {
        result = (score1 > score2) ? 1.0 : (score1 == score2 ? 0.5 : 0.0);
      } else {
        result = (score2 > score1) ? 1.0 : (score1 == score2 ? 0.5 : 0.0);
      }
    }
    if (ai_type == AIType::MCTS_EVAL) {
      result = heuristic_playout(sim_board, sim_p1, sim_p2, sim_turn);
    }
    return result;
  }

  double fast_simulation(Board board, Player p1, Player p2, Color start_turn,
                         int max_steps = 40) {

    static thread_local mt19937 gen(random_device{}());
    Color turn = start_turn;
    int pass_count = 0;

    for (int step = 0; step < max_steps; step++) {

      Player *cur = (turn == Color::PLAYER1) ? &p1 : &p2;

      auto moves = get_fast_legal_moves(board, turn, *cur, 300);

      if (moves.empty()) {
        pass_count++;
        if (pass_count >= 2)
          break;
      } else {
        pass_count = 0;

        uniform_int_distribution<> dis(0, moves.size() - 1);
        auto [id, x, y, rot] = moves[dis(gen)];

        Block blk(getBlock(id));
        blk.rotate_block(rot);

        board.change_status(turn, blk, id, rot, x, y, *cur);
        cout << "Step " << step + 1 << ": "
             << ((turn == Color::PLAYER1) ? "P1" : "P2") << " plays block "
             << id << " at (" << x << "," << y << ") rot=" << rot << "\n";
        // board.print_status(turn);
      }

      turn = (turn == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;
    }

    if (start_turn == Color::PLAYER1) {
      if (p1.score > p2.score)
        cout << "P1 wins\n";
      return 1.0;
      if (p1.score < p2.score) {
        cout << "P2 wins\n";
        return 0.0;
      }
      return 0.5; // 引き分け
    } else {
      if (p2.score > p1.score)
        cout << "P2 wins\n";
      return 1.0;
      if (p2.score < p1.score) {
        cout << "P1 wins\n";
        return 0.0;
      }
      return 0.5; // 引き分け
    }
  }

  std::string color_to_string(Color c) {
    return (c == Color::PLAYER1) ? "P1" : "P2";
  }

  // --- Backpropagation ---
  void backpropagate(double result, Color root_player) {
    MCTSNode *node = this;

    while (node != nullptr) {
      node->visit_count++;

      if (node->current_player == root_player) {
        node->win_score -= result;
      } else {
        node->win_score += result;
      }
      /*if (node->depth != 0) {
      cout << "Backpropagate Node Depth " << node->depth << ": Player "
           << color_to_string(node->current_player) << ": Move "
           << node->move_block_id << " (" << node->move_x << "," << node->move_y
           << ") rot=" << node->move_rot
           << ": visit_count=" << node->visit_count
           << ", win_score=" << node->win_score << "\n";
      node->board.print_status(node->current_player);
    } /* デバッグ用 */

      node = node->parent;
    }
  }
};

std::string color_to_string(Color c) {
  return (c == Color::PLAYER1) ? "P1" : "P2";
}

void print_tree_recursive(MCTSNode *node, int depth, int max_depth, int top_k,
                          const std::string &prefix = "") {
  if (!node || depth > max_depth)
    return;

  // 子ノードを訪問回数順にソート
  std::vector<MCTSNode *> children = node->children;
  std::sort(children.begin(), children.end(), [](MCTSNode *a, MCTSNode *b) {
    return a->visit_count > b->visit_count;
  });

  int limit = std::min((int)children.size(), top_k);

  for (int i = 0; i < limit; i++) {
    MCTSNode *c = children[i];

    double Q = (c->visit_count > 0) ? c->win_score / c->visit_count : 0.0;

    std::cout << prefix << "[D" << depth << " -> " << i << "] "
              << "turn=" << color_to_string(c->current_player) << " "
              << "move=" << c->move_block_id << " (" << c->move_x << ","
              << c->move_y << ")"
              << " rot=" << c->move_rot << " | N=" << c->visit_count
              << " | W=" << c->win_score << " | Q=" << Q
              << " | eval=" << c->eval_value << "\n";

    // 再帰（次の深さへ）
    print_tree_recursive(c, depth + 1, max_depth, top_k, prefix + "     ");
  }
}

void print_tree_2level(MCTSNode *root, int top_children = 3,
                       int top_grandchildren = 3) {

  // --- ルート直下 ---
  std::vector<MCTSNode *> children = root->children;
  std::sort(children.begin(), children.end(), [](MCTSNode *a, MCTSNode *b) {
    return a->visit_count > b->visit_count;
  });

  std::cout << "===== MCTS Tree (2 levels) =====\n";

  int c_limit = std::min((int)children.size(), top_children);

  for (int i = 0; i < c_limit; i++) {
    MCTSNode *c = children[i];
    double Qc = (c->visit_count > 0) ? c->win_score / c->visit_count : 0.0;

    std::cout << "[Root -> " << i << "] "
              << "move=" << c->move_block_id << " (" << c->move_x << ","
              << c->move_y << ")"
              << " rot=" << c->move_rot << " | N=" << c->visit_count
              << " | W=" << c->win_score << " | Q=" << Qc
              << " | eval=" << c->eval_value << "\n";

    // --- 孫ノード ---
    std::vector<MCTSNode *> gchildren = c->children;
    std::sort(gchildren.begin(), gchildren.end(), [](MCTSNode *a, MCTSNode *b) {
      return a->visit_count > b->visit_count;
    });

    int g_limit = std::min((int)gchildren.size(), top_grandchildren);

    for (int j = 0; j < g_limit; j++) {
      MCTSNode *g = gchildren[j];
      double Qg = (g->visit_count > 0) ? g->win_score / g->visit_count : 0.0;

      std::cout << "     -> [" << j << "] "
                << "move=" << g->move_block_id << " (" << g->move_x << ","
                << g->move_y << ")"
                << " rot=" << g->move_rot << " | N=" << g->visit_count
                << " | W=" << g->win_score << " | Q=" << Qg
                << " | eval=" << g->eval_value << "\n";
    }
  }
}

void delete_subtree(MCTSNode *node) {
  if (!node)
    return;
  for (auto child : node->children) {
    delete_subtree(child);
  }
  delete node;
}

// ============================
// MCTS 本体
// ============================
std::tuple<std::string, int, int, int> MCTS(Board root_board, Player root_p1,
                                            Player root_p2, Color root_turn,
                                            int iterations, int MAX_TREE_DEPTH,
                                            AIType ai_type) {
  // --- ルートノード作成 ---
  MCTSNode *root = new MCTSNode(root_board, root_p1, root_p2, root_turn);

  auto phase = root_board.get_phase(root_p1, root_p2);

  // ルートの未展開手のセット
  if (root_turn == Color::PLAYER1)
    root->untried_moves =
        get_all_legal_moves(root_board, Color::PLAYER1, root_p1);
  else
    root->untried_moves =
        get_all_legal_moves(root_board, Color::PLAYER2, root_p2);

  if (root->untried_moves.empty()) {
    std::cout << "[MCTS] No moves available.\n";
    return {"", -1, -1, 0};
  }

  while (!root->untried_moves.empty()) {
    root->expand_node();
  }

  switch (phase) {
  case GamePhase::OPENING:
    iterations = 1500;
    break;
  case GamePhase::MIDDLE:
    iterations = 700;
    break;
  case GamePhase::ENDING:
    iterations = 500;
    break;
  }

  std::cout << "[MCTS] Root legal moves = " << root->children.size()
            << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());

  // ============================
  // MCTSループ
  // ============================
  for (int iter = 0; iter < iterations; iter++) {

    MCTSNode *node = root;
    // 1. Selection
    // cout << "[MCTS] Iteration " << iter + 1 << "/" << iterations << "\n";
    while (node->depth < MAX_TREE_DEPTH && node->untried_moves.empty() &&
           !node->children.empty()) {
      node = node->select_child();
    }

    // cout << "[MCTS] Expansion phase.\n";
    // 2. Expansion
    if (node->depth < MAX_TREE_DEPTH && !node->untried_moves.empty() &&
        node->visit_count > 5) {
      node = node->expand_node();
    }

    double result = 0.0;

    // cout << "[MCTS] Simulation phase.\n";
    // 3. Simulation
    if (ai_type == AIType::MCTS_EVAL) {
      result = node->simulate(ai_type);
    }

    if (ai_type == AIType::MCTS_WIN) {
      result = node->simulate(ai_type);
    }

    node->eval_value = result;

    // cout << "[MCTS] Backpropagation phase.\n";
    // 4. Backpropagation
    node->backpropagate(result, root_turn);
  }

  print_tree_recursive(root,
                       1, // depth（Root直下を1とする）
                       5, // 最大深さ
                       1  // 各階層で上位1ノードのみ表示
  );

  // ============================
  // 最良手を選択
  // ============================
  MCTSNode *best_child = nullptr;
  int best_visit = -1;

  // cout << "[MCTS] Selecting best move from root children.\n";
  for (auto child : root->children) {
    if (child->visit_count > best_visit) {
      best_visit = child->visit_count;
      best_child = child;
    }
  }

  if (best_child == nullptr) {
    std::cout << "[MCTS] ERROR: no best child.\n";
    return {"", -1, -1, 0};
  }

  std::string best_block_id = best_child->move_block_id;
  int best_x = best_child->move_x;
  int best_y = best_child->move_y;
  int best_rot = best_child->move_rot;

  cout << "[MCTS] Best move: " << best_child->move_block_id << " ("
       << best_child->move_x << "," << best_child->move_y
       << ") rot=" << best_child->move_rot << "\n";
  // 最良手を返す

  // ツリー解放
  delete_subtree(root);

  return {best_block_id, best_x, best_y, best_rot};
}

GameResult play_game(Board board, Player p1, Player p2, Color start_turn,
                     AIType p1_ai, AIType p2_ai, int mcts_iterations,
                     int max_tree_depth) {

  Color turn = start_turn;
  int pass_count = 0;

  static std::mt19937 gen(std::random_device{}());

  while (true) {

    Player *current = (turn == Color::PLAYER1) ? &p1 : &p2;
    AIType ai_type = (turn == Color::PLAYER1) ? p1_ai : p2_ai;

    auto legal = get_all_legal_moves(board, turn, *current);

    if (legal.empty()) {
      pass_count++;
      if (pass_count >= 2)
        break;
    } else {
      pass_count = 0;

      std::string block_id;
      int x, y, rot;

      if (ai_type == AIType::MCTS_EVAL) {

        std::tie(block_id, x, y, rot) =
            MCTS(board, p1, p2, turn, mcts_iterations, max_tree_depth, ai_type);

      } else if (ai_type == AIType::MCTS_WIN) {

        std::tie(block_id, x, y, rot) =
            MCTS(board, p1, p2, turn, mcts_iterations, max_tree_depth, ai_type);

      } else if (ai_type == AIType::RANDOM) {

        std::uniform_int_distribution<> dis(0, legal.size() - 1);
        std::tie(block_id, x, y, rot) = legal[dis(gen)];
      }

      if (!block_id.empty()) {
        Block blk(getBlock(block_id));
        board.change_status(turn, blk, block_id, rot, x, y, *current);
        if (turn == Color::PLAYER1) {
          cout << "Turn" << p1.turn_num << "- PLAYER1 placed block " << block_id
               << " at (" << x << "," << y << ") with rotation " << rot << "\n";
          // board.print_status(turn);
        } else {
          cout << "Turn" << p2.turn_num << "- PLAYER2 placed block " << block_id
               << " at (" << x << "," << y << ") with rotation " << rot << "\n";
          // board.print_status(turn);
        }
      }
    }

    turn = (turn == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;
  }

  if (p1.score > p2.score) {
    board.print_status(Color::PLAYER1);
    cout << "Final Score - P1: " << p1.score << ", P2: " << p2.score << "\n";
    cout << "P1 is WIN!"
         << "\n";
    return GameResult::P1_WIN;
  }
  if (p2.score > p1.score) {
    board.print_status(Color::PLAYER2);
    cout << "Final Score - P1: " << p1.score << ", P2: " << p2.score << "\n";
    cout << "P2 is WIN!"
         << "\n";
    return GameResult::P2_WIN;
  }
  return GameResult::DRAW;
}

std::string Aitype_to_string(AIType c) {
  return (c == AIType::MCTS_EVAL)  ? "EVAL"
         : (c == AIType::MCTS_WIN) ? "WIN"
                                   : "RAND";
}

int main() {
  const int TILE_NUMBER = 14;
  const int MAX_TREE_DEPTH = 10;
  int iterations = 300;
  int N = 50;
  AIType p1_ai = AIType::MCTS_EVAL;
  AIType p2_ai = AIType::RANDOM;

  init_block_ids_by_size();

  // --- 盤面初期化 ---
  vector<vector<vector<int>>> input_board = {
      // ...（あなたの貼った盤面をそのまま使用）
      // PLAYER1
      {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
      // PLAYER2
      {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}};

  int win_p1_ai[] = {0, 0};
  int win_p2_ai[] = {0, 0};

  for (int i = 0; i < N; i++) {
    cout << "===== " << Aitype_to_string(p1_ai) << " vs "
         << Aitype_to_string(p2_ai) << " Game " << i + 1 << " =====" << endl;

    Board board(TILE_NUMBER, input_board);
    Player p1{Color::PLAYER1, {""}};
    Player p2{Color::PLAYER2, {""}};
    p1.turn_num = 0;
    p2.turn_num = 0;

    /*cout << "legal moves: "
         << get_all_legal_moves(board, Color::PLAYER1, p1).size() << endl;

    auto [block_id, x, y, rot] = MCTS(board, p1, p2, Color::PLAYER1, iterations,
                                      MAX_TREE_DEPTH, AIType::MCTS_EVAL);*/

    auto result = play_game(board, p1, p2, Color::PLAYER1, p1_ai, p2_ai,
                            iterations, MAX_TREE_DEPTH);

    if (result == GameResult::P1_WIN)
      win_p1_ai[0]++;
    if (result == GameResult::P2_WIN)
      win_p2_ai[0]++;
  }

  for (int i = 0; i < N; i++) {
    cout << "===== " << Aitype_to_string(p2_ai) << " vs "
         << Aitype_to_string(p1_ai) << " Game " << i + 1 << " =====" << endl;

    Board board(TILE_NUMBER, input_board);
    Player p1{Color::PLAYER1, {""}};
    Player p2{Color::PLAYER2, {""}};
    p1.turn_num = 0;
    p2.turn_num = 0;

    /*cout << "legal moves: "
         << get_all_legal_moves(board, Color::PLAYER1, p1).size() << endl;

    auto [block_id, x, y, rot] = MCTS(board, p1, p2, Color::PLAYER1, iterations,
                                      MAX_TREE_DEPTH, AIType::MCTS_EVAL);*/

    auto result = play_game(board, p1, p2, Color::PLAYER1, p2_ai, p1_ai,
                            iterations, MAX_TREE_DEPTH);

    if (result == GameResult::P1_WIN)
      win_p2_ai[1]++;
    if (result == GameResult::P2_WIN)
      win_p1_ai[1]++;
  }
  cout << Aitype_to_string(p1_ai) << " vs " << Aitype_to_string(p2_ai)
       << " results: " << win_p1_ai[0] << " - " << win_p2_ai[0] << endl;
  cout << Aitype_to_string(p2_ai) << " vs " << Aitype_to_string(p1_ai)
       << " results: " << win_p2_ai[1] << " - " << win_p1_ai[1] << endl;

  cout << "=====  Results =====" << endl;
  cout << Aitype_to_string(p1_ai)
       << " win rate = " << (double)(win_p1_ai[0] + win_p1_ai[1]) / (N * 2)
       << endl;
  cout << Aitype_to_string(p2_ai)
       << " win rate = " << (double)(win_p2_ai[0] + win_p2_ai[1]) / (N * 2)
       << endl;
  return 0;
}

int main_legal_test() {
  init_block_ids_by_size();

  // --- 盤面初期化 ---
  vector<vector<vector<int>>> input_board = {
      // ...（あなたの貼った盤面をそのまま使用）
      // PLAYER1
      {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 2, 1, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 1, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 2, 1, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
      // PLAYER2
      {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 3, 1, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 1, 2, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}};

  Board board(TILE_NUMBER, input_board);
  Player p1{Color::PLAYER1, {""}};
  Player p2{Color::PLAYER2, {""}};

  vector<tuple<string, int, int, int>> moves =
      get_fast_legal_moves(board, Color::PLAYER1, p1, 10);
  for (int i = 0; i < 10; i++) {
    auto [block_id, x, y, rot] = moves[i];
    cout << block_id << ", (" << x << "," << y << "), rot:" << rot << endl;
  }
  return 0;
}
