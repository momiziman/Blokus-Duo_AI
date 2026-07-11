#pragma once

#include "board.hpp"

int score_limitter(Player p);
vector<Move> get_all_legal_moves(Board &board, Color player_color,
                                 Player &player);
vector<Move> get_fast_legal_moves(Board &board, Color color, Player &player,
                                  int max_moves);
vector<Move> get_one_legal_moves(Board &board, Color color, Player &player,
                                 int max_moves);
vector<Move> get_oneable_legal_moves(Board &board, Color color,
                                     Player &player);
vector<pair<string, int>> get_legal_moves_no_pos(
    Board &board, Color player_color, Player &player);
vector<string> get_legal_block_types(Board &board, Color player_color,
                                     Player &player);
int mobility(Board &board, Color color, Player &player);
double evaluate(Board &board, Player &p1, Player &p2, Color turn,
                GamePhase phase);
pair<int, int> random_playout(Board board, Player player1, Player player2,
                              Color turn);
double heuristic_playout(Board board, Player p1, Player p2, Color turn);
Move MCTS(Board root_board, Player root_p1, Player root_p2, Color root_turn,
          int iterations, int MAX_TREE_DEPTH, AIType ai_type);

struct GameStats {
  int p1_score = 0;
  int p2_score = 0;
  int p1_turns = 0;
  int p2_turns = 0;
};

GameResult play_game(Board board, Player p1, Player p2, Color start_turn,
                     AIType p1_ai, AIType p2_ai, int mcts_iterations,
                     int max_tree_depth, GameStats *stats = nullptr);
std::string Aitype_to_string(AIType c);
