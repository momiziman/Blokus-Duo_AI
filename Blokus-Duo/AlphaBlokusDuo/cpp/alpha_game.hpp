#pragma once

#include "../../include/game.hpp"

#include <cstdint>
#include <random>
#include <string>
#include <tuple>
#include <vector>

namespace alphablokus {

constexpr int kBoardSize = 14;
constexpr int kPieceCount = 21;
constexpr int kOrientations = 8;
constexpr int kSpatialActions =
    kPieceCount * kOrientations * kBoardSize * kBoardSize;
constexpr int kPassAction = kSpatialActions;
constexpr int kActionSize = kSpatialActions + 1;
constexpr int kObservationChannels = 49;

class AlphaGameState {
public:
  AlphaGameState();

  std::vector<Move> legal_moves();
  std::vector<int> legal_actions();
  void play(const Move &move);
  void play_action(int action);

  bool is_terminal() const noexcept { return terminal_; }
  int current_player() const noexcept {
    return static_cast<int>(current_player_);
  }
  int consecutive_passes() const noexcept { return consecutive_passes_; }
  std::tuple<int, int> scores() const noexcept;
  std::tuple<int, int> turns() const noexcept;
  std::tuple<std::uint32_t, std::uint32_t> used_masks() const noexcept;

  // Result from `perspective`: 1 win, 0 draw, -1 loss.
  float terminal_value(int perspective) const;
  float score_margin_value(int perspective) const;
  std::vector<float> observation() const;
  std::string board_string() const;

  Move select_baseline(const std::string &name, int iterations,
                       int max_tree_depth, std::uint32_t seed = 0);

  static int move_to_action(const Move &move);
  static Move action_to_move(int action);

private:
  static std::vector<std::vector<std::vector<int>>> initial_board();
  Player &current_player_state();
  const Player &player_state(Color color) const;
  bool contains_legal_move(const Move &move, const std::vector<Move> &legal);

  Board board_;
  Player player1_;
  Player player2_;
  Color current_player_ = Color::PLAYER1;
  int consecutive_passes_ = 0;
  bool terminal_ = false;
};

} // namespace alphablokus
