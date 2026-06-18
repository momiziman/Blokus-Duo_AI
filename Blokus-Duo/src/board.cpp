#include "../include/board.hpp"

Board::Board(int tile_number, const vector<vector<vector<int>>> &input_board)
    : TILE_NUMBER(tile_number), status(input_board) {}

bool Board::settable_check(Color color, const vector<vector<int>> &block_shape,
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

vector<pair<int, int>>
  Board::search_settable_position(Color color,
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

vector<pair<int, int>> Board::search_settable_position_near_ableset(
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

vector<pair<int, int>> Board::search_settable_position_one_ableset(
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
  Board::select_random_settable_position(Color color) {
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

void Board::change_status(Color color, Block &block, const std::string &block_id,
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
    auto occupied = block.occupied_offsets();
    for (const auto &offset : occupied) {
      int yy = y + offset.y;
      int xx = x + offset.x;
      if (!in_bounds(yy, xx))
        continue;

      status[col][yy][xx] = MYBLOCK;
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
    for (const auto &offset : occupied) {
        int yy = y + offset.y;
        int xx = x + offset.x;
        if (!in_bounds(yy, xx))
          continue;

        // 相手盤面でも既に OPBLOCK があれば上書きしない
        if (status[opp_col][yy][xx] != OPBLOCK)
          status[opp_col][yy][xx] = OPBLOCK;
    }

    // --- used_blocks に追加 ---
    player.used_blocks.push_back(block_id);
    player.turn_num++;

    // --- スコア加算 ---
    auto it = block_table.find(block_id);
    if (it != block_table.end())
      player.score += it->second.score;
  }

void Board::print_status(Color color) {
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

GamePhase Board::get_phase(const Player &p1, const Player &p2) const {
    int total_score = p1.score + p2.score;

    if (total_score <= 39) {
      return GamePhase::OPENING;
    } else if (total_score <= 99) {
      return GamePhase::MIDDLE;
    } else {
      return GamePhase::ENDING;
    }
  }
