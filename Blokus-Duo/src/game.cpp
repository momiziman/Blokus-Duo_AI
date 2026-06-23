#include "../include/game.hpp"

struct MCTSNode;
void log_node_basic(const MCTSNode *node);

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
vector<Move> get_all_legal_moves(Board &board, Color player_color,
                                 Player &player) {
  vector<Move> legal_moves;

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

vector<Move> get_fast_legal_moves(Board &board, Color color, Player &player,
                                  int max_moves) {

  vector<Move> moves;

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

vector<Move> get_one_legal_moves(Board &board, Color color, Player &player,
                                 int max_moves) {

  vector<Move> moves;

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

vector<Move> get_oneable_legal_moves(Board &board, Color color,
                                     Player &player) {

  vector<Move> moves;

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
    std::tie(ax, ay) = a.value();
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

          Move move = legal_moves[idx];
          BlockData data = getBlock(move.block_id);
          Block block(data);

          board.change_status(current_color, block, move.block_id,
                              move.rotation, move.x, move.y, *current_player);
          // board.print_status(current_color); /*  デバッグ用  */
        }
      }
    } else {
      std::uniform_int_distribution<> dis(0, (int)legal_moves.size() - 1);
      int idx = dis(gen);

      Move move = legal_moves[idx];
      BlockData data = getBlock(move.block_id);
      Block block(data);

      /*cout << "Block id: " << block_id
           << ", legal_moves size: " << legal_moves.size() << endl;/* デバッグ用
       */
      board.change_status(current_color, block, move.block_id, move.rotation,
                          move.x, move.y, *current_player);
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

          Move move = moves[idx];
          BlockData data = getBlock(move.block_id);
          Block block(data);
          board.change_status(turn, block, move.block_id, move.rotation, move.x,
                              move.y, *cur);
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

      Move move = moves[idx];
      BlockData data = getBlock(move.block_id);
      Block block(data);
      board.change_status(turn, block, move.block_id, move.rotation, move.x,
                          move.y, *cur);
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

  std::vector<Move> untried_moves;

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
    Move move = untried_moves[idx];

    // 選んだ手を未展開リストから削除
    untried_moves.erase(untried_moves.begin() + idx);

    // 次ノード用の盤面/プレイヤー状態をコピー
    Board next_board = board;
    Player next_p1 = player1;
    Player next_p2 = player2;

    Player *cur = (current_player == Color::PLAYER1) ? &next_p1 : &next_p2;

    // 盤面更新（next_board に対して）
    BlockData data = getBlock(move.block_id);
    Block blk(data);
    next_board.change_status(current_player, blk, move.block_id, move.rotation,
                             move.x, move.y, *cur);

    // 次ターンを決定
    Color next_turn =
        (current_player == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;

    // 子ノードを生成（parent=this）
    MCTSNode *child =
        new MCTSNode(next_board, next_p1, next_p2, next_turn, this);

    // 子ノードの着手情報を設定
    child->move_block_id = move.block_id;
    child->move_x = move.x;
    child->move_y = move.y;
    child->move_rot = move.rotation;

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
        Move move = moves[dis(gen)];

        Block blk(getBlock(move.block_id));
        blk.rotate_block(move.rotation);

        board.change_status(turn, blk, move.block_id, move.rotation, move.x,
                            move.y, *cur);
        cout << "Step " << step + 1 << ": "
             << ((turn == Color::PLAYER1) ? "P1" : "P2") << " plays block "
             << move.block_id << " at (" << move.x << "," << move.y
             << ") rot=" << move.rotation << "\n";
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
Move MCTS(Board root_board, Player root_p1, Player root_p2, Color root_turn,
          int iterations, int MAX_TREE_DEPTH, AIType ai_type) {
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
    return Move();
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
    return Move();
  }

  Move best_move(best_child->move_block_id, best_child->move_x,
                 best_child->move_y, best_child->move_rot);

  cout << "[MCTS] Best move: " << best_child->move_block_id << " ("
       << best_child->move_x << "," << best_child->move_y
       << ") rot=" << best_child->move_rot << "\n";
  // 最良手を返す

  // ツリー解放
  delete_subtree(root);

  return best_move;
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

      Move move;

      if (ai_type == AIType::MCTS_EVAL) {

        move =
            MCTS(board, p1, p2, turn, mcts_iterations, max_tree_depth, ai_type);

      } else if (ai_type == AIType::MCTS_WIN) {

        move =
            MCTS(board, p1, p2, turn, mcts_iterations, max_tree_depth, ai_type);

      } else if (ai_type == AIType::RANDOM) {

        std::uniform_int_distribution<> dis(0, legal.size() - 1);
        move = legal[dis(gen)];
      }

      if (!move.is_pass()) {
        Block blk(getBlock(move.block_id));
        board.change_status(turn, blk, move.block_id, move.rotation, move.x,
                            move.y, *current);
        if (turn == Color::PLAYER1) {
          cout << "Turn" << p1.turn_num << "- PLAYER1 placed block "
               << move.block_id << " at (" << move.x << "," << move.y
               << ") with rotation " << move.rotation << "\n";
        } else {
          cout << "Turn" << p2.turn_num << "- PLAYER2 placed block "
               << move.block_id << " at (" << move.x << "," << move.y
               << ") with rotation " << move.rotation << "\n";
        }
        board.print_status(turn);
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
