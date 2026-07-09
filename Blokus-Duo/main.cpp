#include "include/game.hpp"

int main() {
  const int TILE_NUMBER = 14;
  const int MAX_TREE_DEPTH = 10;
  int iterations = 300;
  int N = 10;
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
  auto start_time = std::chrono::steady_clock::now();

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

    auto result = play_game(board, p1, p2, Color::PLAYER1, p1_ai, p2_ai,
                            iterations, MAX_TREE_DEPTH);

    if (result == GameResult::P1_WIN)
      win_p1_ai[0]++;
    if (result == GameResult::P2_WIN)
      win_p2_ai[0]++;
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

    auto result = play_game(board, p1, p2, Color::PLAYER1, p2_ai, p1_ai,
                            iterations, MAX_TREE_DEPTH);

    if (result == GameResult::P1_WIN)
      win_p2_ai[1]++;
    if (result == GameResult::P2_WIN)
      win_p1_ai[1]++;
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
  cout << Aitype_to_string(p1_ai)
       << " win rate = " << (double)(win_p1_ai[0] + win_p1_ai[1]) / (N * 2)
       << endl;
  cout << Aitype_to_string(p2_ai)
       << " win rate = " << (double)(win_p2_ai[0] + win_p2_ai[1]) / (N * 2)
       << endl;
  cout << "Elapsed time = " << elapsed.count() << " sec" << endl;
  return 0;
}
