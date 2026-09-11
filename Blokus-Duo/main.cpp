#include "include/game.hpp"

namespace {

void print_final_board(const Board &board) {
  constexpr const char *RESET = "\033[0m";
  constexpr const char *P1_CELL = "\033[44m  \033[0m ";
  constexpr const char *P2_CELL = "\033[41m  \033[0m ";
  constexpr const char *EMPTY_CELL = "\033[90m .\033[0m ";

  cout << "Final board\n"
       << "  PLAYER1: " << P1_CELL << "(blue)  "
       << "PLAYER2: " << P2_CELL << "(red)  "
       << "empty: " << EMPTY_CELL << '\n'
       << RESET << "    ";
  for (int x = 1; x <= board.TILE_NUMBER; ++x)
    cout << std::setw(2) << x << ' ';
  cout << '\n';

  for (int y = 1; y <= board.TILE_NUMBER; ++y) {
    cout << std::setw(2) << y << "  ";
    for (int x = 1; x <= board.TILE_NUMBER; ++x) {
      const uint8_t bits = board.cell_bits(x, y);
      if (bits & Board::P1_BLOCK_BIT)
        cout << P1_CELL;
      else if (bits & Board::P2_BLOCK_BIT)
        cout << P2_CELL;
      else
        cout << EMPTY_CELL;
    }
    cout << '\n';
  }
}

void print_game_result(int game_number, int total_games, AIType p1_ai,
                       AIType p2_ai, GameResult result, const GameStats &stats,
                       const Board &board) {
  cout << "\n===== Game " << game_number << " / " << total_games
       << " =====\n";
  cout << "PLAYER1: " << Aitype_to_string(p1_ai)
       << " | PLAYER2: " << Aitype_to_string(p2_ai) << '\n';

  if (result == GameResult::P1_WIN)
    cout << "Winner: PLAYER1 (" << Aitype_to_string(p1_ai) << ")\n";
  else if (result == GameResult::P2_WIN)
    cout << "Winner: PLAYER2 (" << Aitype_to_string(p2_ai) << ")\n";
  else
    cout << "Result: DRAW\n";

  cout << "Score: PLAYER1=" << stats.p1_score
       << " PLAYER2=" << stats.p2_score << '\n';
  cout << "Turns: PLAYER1=" << stats.p1_turns
       << " PLAYER2=" << stats.p2_turns << '\n';
  print_final_board(board);
}

} // namespace

int main() {
  const int TILE_NUMBER = 14;
  const int MAX_TREE_DEPTH = 10;
  int iterations = 300;
  int N = 10;
  AIType p1_ai = AIType::MCTS_EVAL;
  AIType p2_ai = AIType::MCTS_STANDARD;

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
  int draws = 0;
  long long total_p1_ai_score = 0;
  long long total_p2_ai_score = 0;
  long long total_p1_ai_turns = 0;
  long long total_p2_ai_turns = 0;
  auto start_time = std::chrono::steady_clock::now();
  const int total_games = N * 2;

  for (int i = 0; i < N; i++) {
    Board board(TILE_NUMBER, input_board);
    Player p1{Color::PLAYER1, {""}};
    Player p2{Color::PLAYER2, {""}};
    p1.turn_num = 0;
    p2.turn_num = 0;

    /*cout << "legal moves: "
         << get_all_legal_moves(board, Color::PLAYER1, p1).size() << endl;

    auto [block_id, x, y, rot] = MCTS(board, p1, p2, Color::PLAYER1, iterations,
                                      MAX_TREE_DEPTH, AIType::MCTS_EVAL);*/

    GameStats stats;
    auto result = play_game(board, p1, p2, Color::PLAYER1, p1_ai, p2_ai,
                            iterations, MAX_TREE_DEPTH, &stats, &board);
    print_game_result(i + 1, total_games, p1_ai, p2_ai, result, stats, board);
    total_p1_ai_score += stats.p1_score;
    total_p2_ai_score += stats.p2_score;
    total_p1_ai_turns += stats.p1_turns;
    total_p2_ai_turns += stats.p2_turns;

    if (result == GameResult::P1_WIN)
      win_p1_ai[0]++;
    if (result == GameResult::P2_WIN)
      win_p2_ai[0]++;
    if (result == GameResult::DRAW)
      draws++;
  }

  for (int i = 0; i < N; i++) {
    Board board(TILE_NUMBER, input_board);
    Player p1{Color::PLAYER1, {""}};
    Player p2{Color::PLAYER2, {""}};
    p1.turn_num = 0;
    p2.turn_num = 0;

    /*cout << "legal moves: "
         << get_all_legal_moves(board, Color::PLAYER1, p1).size() << endl;

    auto [block_id, x, y, rot] = MCTS(board, p1, p2, Color::PLAYER1, iterations,
                                      MAX_TREE_DEPTH, AIType::MCTS_EVAL);*/

    GameStats stats;
    auto result = play_game(board, p1, p2, Color::PLAYER1, p2_ai, p1_ai,
                            iterations, MAX_TREE_DEPTH, &stats, &board);
    print_game_result(N + i + 1, total_games, p2_ai, p1_ai, result, stats,
                      board);
    // The configured AIs swap colors in the second half.
    total_p1_ai_score += stats.p2_score;
    total_p2_ai_score += stats.p1_score;
    total_p1_ai_turns += stats.p2_turns;
    total_p2_ai_turns += stats.p1_turns;

    if (result == GameResult::P1_WIN)
      win_p2_ai[1]++;
    if (result == GameResult::P2_WIN)
      win_p1_ai[1]++;
    if (result == GameResult::DRAW)
      draws++;
  }
  auto end_time = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end_time - start_time;

  cout << "=====  Results =====" << endl;
  cout << Aitype_to_string(p1_ai) << " first: " << win_p1_ai[0] << " - "
       << win_p2_ai[0] << endl;
  cout << Aitype_to_string(p2_ai) << " first: " << win_p2_ai[1] << " - "
       << win_p1_ai[1] << endl;
  cout << "total wins " << Aitype_to_string(p1_ai) << " = "
       << win_p1_ai[0] + win_p1_ai[1] << endl;
  cout << "total wins " << Aitype_to_string(p2_ai) << " = "
       << win_p2_ai[0] + win_p2_ai[1] << endl;
  cout << "draws = " << draws << endl;
  cout << "average " << Aitype_to_string(p1_ai)
       << " score = " << (double)total_p1_ai_score / total_games << endl;
  cout << "average " << Aitype_to_string(p2_ai)
       << " score = " << (double)total_p2_ai_score / total_games << endl;
  cout << "average " << Aitype_to_string(p1_ai)
       << " turns = " << (double)total_p1_ai_turns / total_games << endl;
  cout << "average " << Aitype_to_string(p2_ai)
       << " turns = " << (double)total_p2_ai_turns / total_games << endl;
  cout << Aitype_to_string(p1_ai)
       << " win rate = " << (double)(win_p1_ai[0] + win_p1_ai[1]) / (N * 2)
       << endl;
  cout << Aitype_to_string(p2_ai)
       << " win rate = " << (double)(win_p2_ai[0] + win_p2_ai[1]) / (N * 2)
       << endl;
  cout << "Elapsed time = " << elapsed.count() << " sec" << endl;
  return 0;
}
