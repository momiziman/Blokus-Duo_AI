#include "alpha_game.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace alphablokus {

std::vector<std::vector<std::vector<int>>> AlphaGameState::initial_board() {
  std::vector<std::vector<std::vector<int>>> input(
      COLOR_NUM,
      std::vector<std::vector<int>>(BOARD_SIZE,
                                    std::vector<int>(BOARD_SIZE, BLANK)));
  for (int color = 0; color < COLOR_NUM; ++color) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
      input[color][0][i] = CANTSET;
      input[color][BOARD_SIZE - 1][i] = CANTSET;
      input[color][i][0] = CANTSET;
      input[color][i][BOARD_SIZE - 1] = CANTSET;
    }
  }
  input[static_cast<int>(Color::PLAYER1)][5][5] = ABLESET;
  input[static_cast<int>(Color::PLAYER2)][10][10] = ABLESET;
  return input;
}

AlphaGameState::AlphaGameState()
    : board_(kBoardSize, initial_board()), player1_{Color::PLAYER1, {}},
      player2_{Color::PLAYER2, {}} {
  init_block_ids_by_size();
}

Player &AlphaGameState::current_player_state() {
  return current_player_ == Color::PLAYER1 ? player1_ : player2_;
}

const Player &AlphaGameState::player_state(Color color) const {
  return color == Color::PLAYER1 ? player1_ : player2_;
}

std::vector<Move> AlphaGameState::legal_moves() {
  if (terminal_)
    return {};
  auto moves = get_all_legal_moves(board_, current_player_,
                                   current_player_state());
  if (moves.empty())
    moves.emplace_back();
  return moves;
}

std::vector<int> AlphaGameState::legal_actions() {
  auto moves = legal_moves();
  std::vector<int> actions;
  actions.reserve(moves.size());
  for (const Move &move : moves)
    actions.push_back(move_to_action(move));
  std::sort(actions.begin(), actions.end());
  actions.erase(std::unique(actions.begin(), actions.end()), actions.end());
  return actions;
}

bool AlphaGameState::contains_legal_move(const Move &move,
                                         const std::vector<Move> &legal) {
  const std::uint16_t packed = move.packed();
  return std::any_of(legal.begin(), legal.end(), [&](const Move &candidate) {
    return candidate.packed() == packed;
  });
}

void AlphaGameState::play(const Move &move) {
  if (terminal_)
    throw std::runtime_error("cannot play a move in a terminal state");

  auto legal = legal_moves();
  if (!contains_legal_move(move, legal))
    throw std::invalid_argument("move is not legal in the current state");

  if (move.is_pass()) {
    ++consecutive_passes_;
  } else {
    consecutive_passes_ = 0;
    Block block(getBlock(move.block_id));
    board_.change_status(current_player_, block, move.block_id, move.rotation,
                         move.x, move.y, current_player_state());
  }

  if (consecutive_passes_ >= 2)
    terminal_ = true;
  current_player_ = current_player_ == Color::PLAYER1 ? Color::PLAYER2
                                                       : Color::PLAYER1;
}

void AlphaGameState::play_action(int action) { play(action_to_move(action)); }

std::tuple<int, int> AlphaGameState::scores() const noexcept {
  return {player1_.score, player2_.score};
}

std::tuple<int, int> AlphaGameState::turns() const noexcept {
  return {player1_.turn_num, player2_.turn_num};
}

std::tuple<std::uint32_t, std::uint32_t>
AlphaGameState::used_masks() const noexcept {
  return {player1_.used_mask, player2_.used_mask};
}

float AlphaGameState::terminal_value(int perspective) const {
  if (!terminal_)
    throw std::runtime_error("terminal_value requires a terminal state");
  if (perspective != 0 && perspective != 1)
    throw std::invalid_argument("perspective must be 0 or 1");
  const int mine = perspective == 0 ? player1_.score : player2_.score;
  const int theirs = perspective == 0 ? player2_.score : player1_.score;
  return mine > theirs ? 1.0F : mine < theirs ? -1.0F : 0.0F;
}

float AlphaGameState::score_margin_value(int perspective) const {
  if (perspective != 0 && perspective != 1)
    throw std::invalid_argument("perspective must be 0 or 1");
  const int mine = perspective == 0 ? player1_.score : player2_.score;
  const int theirs = perspective == 0 ? player2_.score : player1_.score;
  return std::clamp(static_cast<float>(mine - theirs) /
                        static_cast<float>(MAX_SCORE),
                    -1.0F, 1.0F);
}

std::vector<float> AlphaGameState::observation() const {
  std::vector<float> result(kObservationChannels * kBoardSize * kBoardSize,
                            0.0F);
  const Color opponent = current_player_ == Color::PLAYER1
                             ? Color::PLAYER2
                             : Color::PLAYER1;
  const int own = static_cast<int>(current_player_);
  const int opp = static_cast<int>(opponent);
  const Player &own_player = player_state(current_player_);
  const Player &opp_player = player_state(opponent);

  auto set_plane = [&](int channel, int y, int x, float value) {
    result[(channel * kBoardSize + y) * kBoardSize + x] = value;
  };

  for (int y = 0; y < kBoardSize; ++y) {
    for (int x = 0; x < kBoardSize; ++x) {
      const int board_y = y + 1;
      const int board_x = x + 1;
      set_plane(0, y, x,
                board_.status[own][board_y][board_x] == MYBLOCK ? 1.0F
                                                                : 0.0F);
      set_plane(1, y, x,
                board_.status[opp][board_y][board_x] == MYBLOCK ? 1.0F
                                                                : 0.0F);
      set_plane(2, y, x,
                board_.status[own][board_y][board_x] == ABLESET ? 1.0F
                                                                : 0.0F);
      set_plane(3, y, x,
                board_.status[opp][board_y][board_x] == ABLESET ? 1.0F
                                                                : 0.0F);
      for (int piece = 0; piece < kPieceCount; ++piece) {
        set_plane(4 + piece, y, x,
                  (own_player.used_mask & (1U << piece)) == 0 ? 1.0F : 0.0F);
        set_plane(25 + piece, y, x,
                  (opp_player.used_mask & (1U << piece)) == 0 ? 1.0F : 0.0F);
      }
      set_plane(46, y, x,
                static_cast<float>(own_player.score) /
                    static_cast<float>(MAX_SCORE));
      set_plane(47, y, x,
                static_cast<float>(opp_player.score) /
                    static_cast<float>(MAX_SCORE));
      set_plane(48, y, x, static_cast<float>(consecutive_passes_) / 2.0F);
    }
  }
  return result;
}

std::string AlphaGameState::board_string() const {
  std::ostringstream out;
  for (int y = 1; y <= kBoardSize; ++y) {
    for (int x = 1; x <= kBoardSize; ++x) {
      const std::uint8_t bits = board_.cell_bits(x, y);
      char cell = '.';
      if (bits & Board::P1_BLOCK_BIT)
        cell = '1';
      else if (bits & Board::P2_BLOCK_BIT)
        cell = '2';
      out << cell << (x == kBoardSize ? '\n' : ' ');
    }
  }
  return out.str();
}

Move AlphaGameState::select_baseline(const std::string &name, int iterations,
                                     int max_tree_depth, std::uint32_t seed) {
  auto legal = legal_moves();
  if (legal.empty() || legal.front().is_pass())
    return Move();
  if (name == "random") {
    std::mt19937 generator(seed == 0 ? std::random_device{}() : seed);
    std::uniform_int_distribution<std::size_t> distribution(0,
                                                            legal.size() - 1);
    return legal[distribution(generator)];
  }
  if (name == "standard")
    return MCTSStandard(board_, player1_, player2_, current_player_, iterations,
                        max_tree_depth);
  if (name == "eval")
    return MCTS(board_, player1_, player2_, current_player_, iterations,
                max_tree_depth, AIType::MCTS_EVAL);
  throw std::invalid_argument("baseline must be random, standard, or eval");
}

int AlphaGameState::move_to_action(const Move &move) {
  if (move.is_pass())
    return kPassAction;
  if (!move.is_valid() || move.x < 1 || move.x > kBoardSize || move.y < 1 ||
      move.y > kBoardSize)
    throw std::invalid_argument("move cannot be encoded as an AlphaZero action");
  const int piece = move.block_id[0] - 'a';
  return (((piece * kOrientations + move.rotation) * kBoardSize +
           (move.y - 1)) *
              kBoardSize +
          (move.x - 1));
}

Move AlphaGameState::action_to_move(int action) {
  if (action == kPassAction)
    return Move();
  if (action < 0 || action >= kSpatialActions)
    throw std::out_of_range("action index is outside the policy space");
  int value = action;
  const int x = value % kBoardSize + 1;
  value /= kBoardSize;
  const int y = value % kBoardSize + 1;
  value /= kBoardSize;
  const int rotation = value % kOrientations;
  const int piece = value / kOrientations;
  return Move(std::string(1, static_cast<char>('a' + piece)), x, y, rotation);
}

} // namespace alphablokus
