#pragma once

#include "piece.hpp"
#include "player.hpp"

class Board {
public:
  inline static const int BLANK = 0;
  inline static const int CANTSET = 1;
  inline static const int ABLESET = 2;
  inline static const int BLOCK = 3;
  inline static const int OPBLOCK = 4;

  inline static const uint8_t P1_BLOCK_BIT = 1 << 0;
  inline static const uint8_t P2_BLOCK_BIT = 1 << 1;
  inline static const uint8_t P1_CANT_BIT = 1 << 2;
  inline static const uint8_t P2_CANT_BIT = 1 << 3;
  inline static const uint8_t P1_ABLE_BIT = 1 << 4;
  inline static const uint8_t P2_ABLE_BIT = 1 << 5;

  int TILE_NUMBER;
  vector<vector<vector<int>>> status;
  vector<vector<uint8_t>> bit_status;

  Board(int tile_number, const vector<vector<vector<int>>> &input_board);

  void rebuild_bit_status();
  uint8_t cell_bits(int x, int y) const;
  bool settable_check(Color color, const vector<vector<int>> &block_shape,
                      int x, int y);
  vector<pair<int, int>> search_settable_position(
      Color color, const vector<vector<int>> &block_shape);
  vector<pair<int, int>> search_settable_position_near_ableset(
      Color color, const vector<vector<int>> &block_shape);
  vector<pair<int, int>> search_settable_position_one_ableset(
      Color color, const vector<vector<int>> &block_shape, int ax, int ay);
  std::optional<std::pair<int, int>> select_random_settable_position(
      Color color);
  void change_status(Color color, Block &block, const std::string &block_id,
                     int rotation, int x, int y, Player &player);
  void print_status(Color color);
  GamePhase get_phase(const Player &p1, const Player &p2) const;
};
