#include "../include/board.hpp"

Board::Board(int tile_number, const vector<vector<vector<int>>> &input_board)
    : TILE_NUMBER(tile_number) {
  for (int col = 0; col < COLOR_NUM; ++col) {
    for (int y = 0; y < BOARD_SIZE; ++y) {
      for (int x = 0; x < BOARD_SIZE; ++x) {
        status[col][y][x] = input_board[col][y][x];
      }
    }
  }
  rebuild_bit_status();
}

void Board::rebuild_bit_status() {
  for (auto &row : bit_status)
    row.fill(0);

  for (int col = 0; col < COLOR_NUM; ++col) {
    uint8_t block_bit = (col == 0) ? P1_BLOCK_BIT : P2_BLOCK_BIT;
    uint8_t opp_block_bit = (col == 0) ? P2_BLOCK_BIT : P1_BLOCK_BIT;
    uint8_t cant_bit = (col == 0) ? P1_CANT_BIT : P2_CANT_BIT;
    uint8_t able_bit = (col == 0) ? P1_ABLE_BIT : P2_ABLE_BIT;

    for (int y = 0; y < TILE_NUMBER + 2; ++y) {
      for (int x = 0; x < TILE_NUMBER + 2; ++x) {
        int cell = status[col][y][x];
        if (cell == MYBLOCK)
          bit_status[y][x] |= block_bit;
        else if (cell == OPBLOCK)
          bit_status[y][x] |= opp_block_bit;
        else if (cell == CANTSET)
          bit_status[y][x] |= cant_bit;
        else if (cell == ABLESET)
          bit_status[y][x] |= able_bit;
      }
    }
  }
}

uint8_t Board::cell_bits(int x, int y) const {
  if (y < 0 || y >= (int)bit_status.size())
    return 0;
  if (x < 0 || x >= (int)bit_status[y].size())
    return 0;
  return bit_status[y][x];
}

void Board::set_cell_state(int color_index, int y, int x, int state) {
  status[color_index][y][x] = state;

  uint8_t own_block_bit = (color_index == 0) ? P1_BLOCK_BIT : P2_BLOCK_BIT;
  uint8_t opp_block_bit = (color_index == 0) ? P2_BLOCK_BIT : P1_BLOCK_BIT;
  uint8_t cant_bit = (color_index == 0) ? P1_CANT_BIT : P2_CANT_BIT;
  uint8_t able_bit = (color_index == 0) ? P1_ABLE_BIT : P2_ABLE_BIT;

  bit_status[y][x] &= static_cast<uint8_t>(~(cant_bit | able_bit));

  if (state == MYBLOCK)
    bit_status[y][x] |= own_block_bit;
  else if (state == OPBLOCK)
    bit_status[y][x] |= opp_block_bit;
  else if (state == CANTSET)
    bit_status[y][x] |= cant_bit;
  else if (state == ABLESET)
    bit_status[y][x] |= able_bit;
}

static Block block_from_shape(const vector<vector<int>> &block_shape) {
  BlockData data{};
  for (int y = 0; y < (int)block_shape.size() && y < 5; ++y) {
    for (int x = 0; x < (int)block_shape[y].size() && x < 5; ++x) {
      data.shape[y][x] = block_shape[y][x];
    }
  }
  return Block(data);
}

bool Board::settable_check(Color color, const Block &block, int x, int y) {
  int col = static_cast<int>(color);
  bool found_corner = false;

  uint8_t own_block_bit = (col == 0) ? P1_BLOCK_BIT : P2_BLOCK_BIT;
  uint8_t opp_block_bit = (col == 0) ? P2_BLOCK_BIT : P1_BLOCK_BIT;
  uint8_t cant_bit = (col == 0) ? P1_CANT_BIT : P2_CANT_BIT;
  uint8_t able_bit = (col == 0) ? P1_ABLE_BIT : P2_ABLE_BIT;

  for (const auto &offset : block.occupied_offsets()) {
    int access_y = y + offset.y;
    int access_x = x + offset.x;

    if (access_y < 0 || access_y >= TILE_NUMBER + 2 || access_x < 0 ||
        access_x >= TILE_NUMBER + 2)
      continue;

    uint8_t bits = bit_status[access_y][access_x];
    if (bits & (own_block_bit | opp_block_bit | cant_bit))
      return false;
    if (bits & able_bit)
      found_corner = true;
  }

  return found_corner;
}

bool Board::settable_check(Color color, const vector<vector<int>> &block_shape,
                           int x, int y) {
  Block block = block_from_shape(block_shape);
  return settable_check(color, block, x, y);
}

vector<pair<int, int>> Board::collect_able_positions(Color color) const {
  vector<pair<int, int>> positions;
  int col = static_cast<int>(color);
  uint8_t able_bit = (col == 0) ? P1_ABLE_BIT : P2_ABLE_BIT;

  for (int y = 1; y <= TILE_NUMBER; ++y) {
    for (int x = 1; x <= TILE_NUMBER; ++x) {
      if (bit_status[y][x] & able_bit)
        positions.emplace_back(x, y);
    }
  }

  return positions;
}

vector<pair<int, int>> Board::search_settable_position(Color color,
                                                       const Block &block) {
  vector<pair<int, int>> res;
  int col = static_cast<int>(color);
  uint8_t cant_bit = (col == 0) ? P1_CANT_BIT : P2_CANT_BIT;

  for (int y = 1; y <= TILE_NUMBER; ++y) {
    for (int x = 1; x <= TILE_NUMBER; ++x) {
      if (!(bit_status[y][x] & cant_bit) && settable_check(color, block, x, y))
        res.push_back({x, y});
    }
  }

  return res;
}

vector<pair<int, int>>
Board::search_settable_position(Color color,
                                const vector<vector<int>> &block_shape) {
  Block block = block_from_shape(block_shape);
  return search_settable_position(color, block);
}

vector<pair<int, int>>
Board::search_settable_position_near_ableset(Color color, const Block &block) {
  return search_settable_position_near_ableset(
      color, block, collect_able_positions(color));
}

vector<pair<int, int>> Board::search_settable_position_near_ableset(
    Color color, const Block &block,
    const vector<pair<int, int>> &able_positions) {
  vector<pair<int, int>> res;
  int col = static_cast<int>(color);
  uint8_t cant_bit = (col == 0) ? P1_CANT_BIT : P2_CANT_BIT;

  bool candidates[BOARD_SIZE][BOARD_SIZE] = {};

  for (const auto &[x, y] : able_positions) {
    for (int dy = -2; dy <= 2; ++dy) {
      for (int dx = -2; dx <= 2; ++dx) {
        int ny = y + dy;
        int nx = x + dx;
        if (1 <= ny && ny <= TILE_NUMBER && 1 <= nx && nx <= TILE_NUMBER)
          candidates[ny][nx] = true;
      }
    }
  }

  for (int y = 1; y <= TILE_NUMBER; ++y) {
    for (int x = 1; x <= TILE_NUMBER; ++x) {
      if (candidates[y][x] && !(bit_status[y][x] & cant_bit) &&
          settable_check(color, block, x, y)) {
        res.push_back({x, y});
      }
    }
  }

  return res;
}

vector<pair<int, int>> Board::search_settable_position_near_ableset(
    Color color, const vector<vector<int>> &block_shape) {
  Block block = block_from_shape(block_shape);
  return search_settable_position_near_ableset(color, block);
}

vector<pair<int, int>> Board::search_settable_position_one_ableset(
    Color color, const Block &block, int ax, int ay) {
  vector<pair<int, int>> res;
  int col = static_cast<int>(color);
  uint8_t able_bit = (col == 0) ? P1_ABLE_BIT : P2_ABLE_BIT;
  uint8_t cant_bit = (col == 0) ? P1_CANT_BIT : P2_CANT_BIT;

  if (!(bit_status[ay][ax] & able_bit)) {
    // cout << "Error:It's not ABLESET! (by search settable position one "
    //         "ableset())"
    //      << endl;
    return res;
  }

  bool candidates[BOARD_SIZE][BOARD_SIZE] = {};
  for (int dy = -2; dy <= 2; ++dy) {
    for (int dx = -2; dx <= 2; ++dx) {
      int ny = ay + dy;
      int nx = ax + dx;
      if (1 <= ny && ny <= TILE_NUMBER && 1 <= nx && nx <= TILE_NUMBER)
        candidates[ny][nx] = true;
    }
  }

  for (int cy = 1; cy <= TILE_NUMBER; ++cy) {
    for (int cx = 1; cx <= TILE_NUMBER; ++cx) {
      if (candidates[cy][cx] && !(bit_status[cy][cx] & cant_bit) &&
          settable_check(color, block, cx, cy)) {
        res.emplace_back(cx, cy);
      }
    }
  }

  return res;
}

vector<pair<int, int>> Board::search_settable_position_one_ableset(
    Color color, const vector<vector<int>> &block_shape, int ax, int ay) {
  Block block = block_from_shape(block_shape);
  return search_settable_position_one_ableset(color, block, ax, ay);
}

std::optional<std::pair<int, int>>
Board::select_random_settable_position(Color color) {
  int col = static_cast<int>(color);
  uint8_t able_bit = (col == 0) ? P1_ABLE_BIT : P2_ABLE_BIT;
  std::optional<std::pair<int, int>> selected;
  int count = 0;
  static thread_local std::mt19937 gen(std::random_device{}());

  for (int y = 1; y <= TILE_NUMBER; ++y) {
    for (int x = 1; x <= TILE_NUMBER; ++x) {
      if (!(bit_status[y][x] & able_bit))
        continue;

      ++count;
      std::uniform_int_distribution<> dist(1, count);
      if (dist(gen) == 1)
        selected = std::make_pair(x, y);
    }
  }

  if (!selected) {
    // cout << "No one ABLESET." << endl;
    return std::nullopt;
  }

  return selected;
}

void Board::change_status(Color color, Block &block, const std::string &block_id,
                          int rotation, int x, int y, Player &player) {
  block.rotate_block(rotation);

  int col = static_cast<int>(color);
  int opp_col = 1 - col;
  int H = status[0].size();
  int W = status[0][0].size();

  auto in_bounds = [&](int yy, int xx) {
    return (0 <= yy && yy < H && 0 <= xx && xx < W);
  };

  const auto &occupied = block.occupied_offsets();
  for (const auto &offset : occupied) {
    int yy = y + offset.y;
    int xx = x + offset.x;
    if (in_bounds(yy, xx))
      set_cell_state(col, yy, xx, MYBLOCK);
  }

  static const int side_dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  static const int edge_dirs[4][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};

  for (const auto &offset : occupied) {
    int base_y = y + offset.y;
    int base_x = x + offset.x;
    for (const auto &dir : side_dirs) {
      int yy = base_y + dir[1];
      int xx = base_x + dir[0];
      if (!in_bounds(yy, xx))
        continue;
      if (status[col][yy][xx] == MYBLOCK || status[col][yy][xx] == OPBLOCK)
        continue;
      set_cell_state(col, yy, xx, CANTSET);
    }
  }

  for (const auto &offset : occupied) {
    int base_y = y + offset.y;
    int base_x = x + offset.x;
    for (const auto &dir : edge_dirs) {
      int yy = base_y + dir[1];
      int xx = base_x + dir[0];
      if (!in_bounds(yy, xx))
        continue;
      if (status[col][yy][xx] == BLANK)
        set_cell_state(col, yy, xx, ABLESET);
    }
  }

  for (const auto &offset : occupied) {
    int yy = y + offset.y;
    int xx = x + offset.x;
    if (!in_bounds(yy, xx))
      continue;

    if (status[opp_col][yy][xx] != OPBLOCK)
      set_cell_state(opp_col, yy, xx, OPBLOCK);
  }

  player.used_blocks.push_back(block_id);
  mark_block_used(player, block_id);
  player.turn_num++;

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
      case 0:
        cout << ". ";
        break;
      case 1:
        cout << "# ";
        break;
      case 2:
        cout << "* ";
        break;
      case 3:
        cout << "\033[32m"
             << "3 "
             << "\033[0m";
        break;
      case 4:
        cout << "\033[31m"
             << "4 "
             << "\033[0m";
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
