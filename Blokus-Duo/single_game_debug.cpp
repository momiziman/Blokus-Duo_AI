#include "include/game.hpp"

#include <fstream>
#include <iomanip>
#include <map>

namespace {

constexpr int MAX_TREE_DEPTH = 10;
constexpr int MCTS_ITERATIONS = 100;
constexpr AIType P1_AI = AIType::RANDOM;
constexpr AIType P2_AI = AIType::RANDOM;
constexpr unsigned RANDOM_SEED = 20260627;

vector<vector<vector<int>>> create_initial_board() {
  vector<vector<vector<int>>> input_board(
      COLOR_NUM, vector<vector<int>>(BOARD_SIZE, vector<int>(BOARD_SIZE, 0)));

  for (int col = 0; col < COLOR_NUM; ++col) {
    for (int y = 0; y < BOARD_SIZE; ++y) {
      for (int x = 0; x < BOARD_SIZE; ++x) {
        if (y == 0 || y == BOARD_SIZE - 1 || x == 0 || x == BOARD_SIZE - 1)
          input_board[col][y][x] = CANTSET;
      }
    }
  }

  input_board[static_cast<int>(Color::PLAYER1)][5][5] = ABLESET;
  input_board[static_cast<int>(Color::PLAYER2)][10][10] = ABLESET;
  return input_board;
}

std::string phase_to_string(GamePhase phase) {
  switch (phase) {
  case GamePhase::OPENING:
    return "OPENING";
  case GamePhase::MIDDLE:
    return "MIDDLE";
  case GamePhase::ENDING:
    return "ENDING";
  }
  return "UNKNOWN";
}

std::string color_to_string_local(Color color) {
  return (color == Color::PLAYER1) ? "PLAYER1" : "PLAYER2";
}

void print_board(std::ostream &os, const Board &board, Color color) {
  int col = static_cast<int>(color);
  for (int y = 0; y < BOARD_SIZE; ++y) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
      char symbol = '?';
      switch (board.status[col][y][x]) {
      case BLANK:
        symbol = '.';
        break;
      case CANTSET:
        symbol = '#';
        break;
      case ABLESET:
        symbol = '*';
        break;
      case MYBLOCK:
        symbol = 'M';
        break;
      case OPBLOCK:
        symbol = 'O';
        break;
      }
      os << symbol << ' ';
    }
    os << '\n';
  }
}

struct BoardDebugStats {
  int blank = 0;
  int cant = 0;
  int able = 0;
  int my_block = 0;
  int op_block = 0;
  int bit_my_block = 0;
  int bit_op_block = 0;
  int bit_cant = 0;
  int bit_able = 0;
  int bit_able_and_cant = 0;
};

BoardDebugStats collect_board_debug_stats(const Board &board, Color color) {
  BoardDebugStats stats;
  int col = static_cast<int>(color);
  uint8_t own_block_bit = (col == 0) ? Board::P1_BLOCK_BIT : Board::P2_BLOCK_BIT;
  uint8_t opp_block_bit = (col == 0) ? Board::P2_BLOCK_BIT : Board::P1_BLOCK_BIT;
  uint8_t cant_bit = (col == 0) ? Board::P1_CANT_BIT : Board::P2_CANT_BIT;
  uint8_t able_bit = (col == 0) ? Board::P1_ABLE_BIT : Board::P2_ABLE_BIT;

  for (int y = 0; y < BOARD_SIZE; ++y) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
      switch (board.status[col][y][x]) {
      case BLANK:
        ++stats.blank;
        break;
      case CANTSET:
        ++stats.cant;
        break;
      case ABLESET:
        ++stats.able;
        break;
      case MYBLOCK:
        ++stats.my_block;
        break;
      case OPBLOCK:
        ++stats.op_block;
        break;
      }

      uint8_t bits = board.cell_bits(x, y);
      if (bits & own_block_bit)
        ++stats.bit_my_block;
      if (bits & opp_block_bit)
        ++stats.bit_op_block;
      if (bits & cant_bit)
        ++stats.bit_cant;
      if (bits & able_bit)
        ++stats.bit_able;
      if ((bits & cant_bit) && (bits & able_bit))
        ++stats.bit_able_and_cant;
    }
  }

  return stats;
}

int count_bit_status_exact_mismatches(const Board &board) {
  int mismatches = 0;
  for (int y = 0; y < BOARD_SIZE; ++y) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
      uint8_t expected = 0;

      for (int col = 0; col < COLOR_NUM; ++col) {
        uint8_t block_bit =
            (col == 0) ? Board::P1_BLOCK_BIT : Board::P2_BLOCK_BIT;
        uint8_t opp_block_bit =
            (col == 0) ? Board::P2_BLOCK_BIT : Board::P1_BLOCK_BIT;
        uint8_t cant_bit =
            (col == 0) ? Board::P1_CANT_BIT : Board::P2_CANT_BIT;
        uint8_t able_bit =
            (col == 0) ? Board::P1_ABLE_BIT : Board::P2_ABLE_BIT;

        int cell = board.status[col][y][x];
        if (cell == MYBLOCK)
          expected |= block_bit;
        else if (cell == OPBLOCK)
          expected |= opp_block_bit;
        else if (cell == CANTSET)
          expected |= cant_bit;
        else if (cell == ABLESET)
          expected |= able_bit;
      }

      if (board.cell_bits(x, y) != expected)
        ++mismatches;
    }
  }
  return mismatches;
}

void log_board_debug_stats(std::ostream &os, const Board &board, Color color,
                           const std::string &label) {
  BoardDebugStats stats = collect_board_debug_stats(board, color);
  os << label << " " << color_to_string_local(color)
     << " status(blank/cant/able/my/op)=" << stats.blank << "/"
     << stats.cant << "/" << stats.able << "/" << stats.my_block << "/"
     << stats.op_block << " bit(my/op/cant/able)=" << stats.bit_my_block
     << "/" << stats.bit_op_block << "/" << stats.bit_cant << "/"
     << stats.bit_able
     << " bit_able_and_cant=" << stats.bit_able_and_cant << '\n';
}

void log_legal_moves_by_block(std::ostream &os,
                              const vector<Move> &legal_moves) {
  std::map<std::string, int> counts;
  for (const auto &move : legal_moves)
    ++counts[move.block_id];

  os << "legal moves by block:";
  if (counts.empty()) {
    os << " none\n";
    return;
  }

  for (const auto &[block_id, count] : counts)
    os << ' ' << block_id << '=' << count;
  os << '\n';
}

void log_unused_blocks_with_legal_counts(std::ostream &os, const Player &player,
                                         const vector<Move> &legal_moves) {
  std::map<std::string, int> counts;
  for (const auto &move : legal_moves)
    ++counts[move.block_id];

  os << "unused blocks legal counts:";
  bool any = false;
  for (const auto &[block_id, _] : block_table) {
    if (is_block_used(player, block_id))
      continue;
    any = true;
    os << ' ' << block_id << '=' << counts[block_id];
  }
  if (!any)
    os << " none";
  os << '\n';
}

bool check_bit_status_consistency(const Board &board) {
  for (int y = 0; y < BOARD_SIZE; ++y) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
      uint8_t expected = 0;

      for (int col = 0; col < COLOR_NUM; ++col) {
        uint8_t block_bit =
            (col == 0) ? Board::P1_BLOCK_BIT : Board::P2_BLOCK_BIT;
        uint8_t opp_block_bit =
            (col == 0) ? Board::P2_BLOCK_BIT : Board::P1_BLOCK_BIT;
        uint8_t cant_bit =
            (col == 0) ? Board::P1_CANT_BIT : Board::P2_CANT_BIT;
        uint8_t able_bit =
            (col == 0) ? Board::P1_ABLE_BIT : Board::P2_ABLE_BIT;

        int cell = board.status[col][y][x];
        if (cell == MYBLOCK)
          expected |= block_bit;
        else if (cell == OPBLOCK)
          expected |= opp_block_bit;
        else if (cell == CANTSET)
          expected |= cant_bit;
        else if (cell == ABLESET)
          expected |= able_bit;
      }

      if (board.cell_bits(x, y) != expected)
        return false;
    }
  }
  return true;
}

void log_move(std::ostream &os, const Move &move) {
  os << "move block=" << move.block_id << " x=" << move.x << " y=" << move.y
     << " rotation=" << move.rotation << " packed=0x" << std::hex
     << std::setw(4) << std::setfill('0') << move.packed() << std::dec
     << std::setfill(' ') << '\n';
}

Move choose_move(Board &board, Player &p1, Player &p2, Color turn,
                 AIType ai_type, const vector<Move> &legal_moves,
                 std::mt19937 &gen, std::ostream &log) {
  if (legal_moves.empty())
    return Move();

  if (ai_type == AIType::RANDOM) {
    std::uniform_int_distribution<> dis(0, (int)legal_moves.size() - 1);
    Move move = legal_moves[dis(gen)];
    log << "selection method=RANDOM\n";
    return move;
  }

  log << "selection method=MCTS iterations=" << MCTS_ITERATIONS
      << " max_depth=" << MAX_TREE_DEPTH << '\n';
  return MCTS(board, p1, p2, turn, MCTS_ITERATIONS, MAX_TREE_DEPTH, ai_type);
}

} // namespace

int main() {
  std::ofstream log("single_game_debug_log.txt");
  if (!log) {
    std::cerr << "Failed to open single_game_debug_log.txt\n";
    return 1;
  }

  init_block_ids_by_size();

  Board board(TILE_NUMBER, create_initial_board());
  Player p1{Color::PLAYER1, {""}};
  Player p2{Color::PLAYER2, {""}};
  p1.turn_num = 0;
  p2.turn_num = 0;

  std::mt19937 gen(RANDOM_SEED);
  Color turn = Color::PLAYER1;
  int pass_count = 0;
  int step = 1;

  log << "===== Single Game Debug Start =====\n";
  log << "P1 AI=" << Aitype_to_string(P1_AI)
      << " P2 AI=" << Aitype_to_string(P2_AI) << '\n';
  log << "random seed=" << RANDOM_SEED << "\n\n";

  while (pass_count < 2) {
    Player *current = (turn == Color::PLAYER1) ? &p1 : &p2;
    AIType ai_type = (turn == Color::PLAYER1) ? P1_AI : P2_AI;
    GamePhase phase = board.get_phase(p1, p2);

    log << "----- Step " << step << " -----\n";
    log << "turn=" << color_to_string_local(turn)
        << " ai=" << Aitype_to_string(ai_type)
        << " phase=" << phase_to_string(phase) << '\n';
    log << "score before P1=" << p1.score << " P2=" << p2.score << '\n';
    log << "used blocks count=" << current->used_blocks.size() - 1 << '\n';
    log_board_debug_stats(log, board, Color::PLAYER1, "board stats before");
    log_board_debug_stats(log, board, Color::PLAYER2, "board stats before");
    log << "bit_status exact mismatches before="
        << count_bit_status_exact_mismatches(board) << '\n';

    auto legal_moves = get_all_legal_moves(board, turn, *current);
    log << "legal moves=" << legal_moves.size() << '\n';
    log_legal_moves_by_block(log, legal_moves);
    log_unused_blocks_with_legal_counts(log, *current, legal_moves);
    for (int i = 0; i < std::min<int>(5, legal_moves.size()); ++i) {
      log << "  candidate[" << i << "] ";
      log_move(log, legal_moves[i]);
    }

    if (legal_moves.empty()) {
      pass_count++;
      log << "result=PASS pass_count=" << pass_count << "\n\n";
      turn = (turn == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;
      ++step;
      continue;
    }

    pass_count = 0;
    Move move =
        choose_move(board, p1, p2, turn, ai_type, legal_moves, gen, log);
    log << "selected ";
    log_move(log, move);
    log << "roundtrip ";
    log_move(log, Move::from_packed(move.packed()));

    Block block(getBlock(move.block_id));
    board.change_status(turn, block, move.block_id, move.rotation, move.x,
                        move.y, *current);

    log << "score after P1=" << p1.score << " P2=" << p2.score << '\n';
    log << "bit_status consistency="
        << (check_bit_status_consistency(board) ? "OK" : "NG") << '\n';
    log_board_debug_stats(log, board, Color::PLAYER1, "board stats after");
    log_board_debug_stats(log, board, Color::PLAYER2, "board stats after");
    log << "bit_status exact mismatches after="
        << count_bit_status_exact_mismatches(board) << '\n';
    log << "board view for " << color_to_string_local(turn) << ":\n";
    print_board(log, board, turn);
    log << '\n';

    turn = (turn == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;
    ++step;
  }

  log << "===== Single Game Debug Result =====\n";
  log << "final score P1=" << p1.score << " P2=" << p2.score << '\n';
  log << "final turns P1=" << p1.turn_num << " P2=" << p2.turn_num << '\n';
  log << "final used blocks P1=" << p1.used_blocks.size() - 1
      << " P2=" << p2.used_blocks.size() - 1 << '\n';
  log << "final used mask P1=0x" << std::hex << p1.used_mask
      << " P2=0x" << p2.used_mask << std::dec << '\n';
  if (p1.score > p2.score)
    log << "winner=PLAYER1\n";
  else if (p2.score > p1.score)
    log << "winner=PLAYER2\n";
  else
    log << "winner=DRAW\n";
  log << "\nfinal board view for PLAYER1:\n";
  print_board(log, board, Color::PLAYER1);
  log << "\nfinal board view for PLAYER2:\n";
  print_board(log, board, Color::PLAYER2);

  std::cout << "single_game_debug_log.txt was created.\n";
  return 0;
}
