#include "../include/piece.hpp"

const std::unordered_map<std::string, BlockData> block_table = {
    // shape (5x5)
    {"a",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      1}}, // score
    {"b",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      2}}, // score
    {"c",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      3}}, // score
    {"d",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      3}}, // score
    {"e",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0}}},
      4}}, // score
    {"f",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"g",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"h",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"i",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 2, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"j",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0}}},
      5}}, // score
    {"k",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"l",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 2, 0, 0},
        {0, 2, 1, 2, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"m",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"n",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"o",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0}}},
      5}}, // score
    {"p",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"q",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 2},
        {0, 0, 1, 1, 1, 1, 1},
        {0, 0, 2, 1, 1, 1, 2},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"r",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 2, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 1, 0},
        {0, 0, 0, 2, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"s",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 2, 0, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 1, 0},
        {0, 0, 0, 2, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"t",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 2, 0, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"u",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score

};

const std::unordered_map<std::string, std::array<std::vector<Position>, 8>>
    block_rotation_table = {
        {"a", {{
            std::vector<Position>{{0, 0}},
            std::vector<Position>{{0, 0}},
            std::vector<Position>{{0, 0}},
            std::vector<Position>{{0, 0}},
            std::vector<Position>{{0, 0}},
            std::vector<Position>{{0, 0}},
            std::vector<Position>{{0, 0}},
            std::vector<Position>{{0, 0}}
        }}},
        {"b", {{
            std::vector<Position>{{0, 0}, {0, 1}},
            std::vector<Position>{{0, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}},
            std::vector<Position>{{0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {0, 0}},
            std::vector<Position>{{0, -1}, {0, 0}},
            std::vector<Position>{{0, 0}, {1, 0}},
            std::vector<Position>{{-1, 0}, {0, 0}}
        }}},
        {"c", {{
            std::vector<Position>{{0, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}}
        }}},
        {"d", {{
            std::vector<Position>{{0, -1}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}},
            std::vector<Position>{{0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {0, 1}},
            std::vector<Position>{{0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}},
            std::vector<Position>{{0, -1}, {0, 0}, {1, 0}}
        }}},
        {"e", {{
            std::vector<Position>{{0, -1}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{0, -1}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {1, 0}}
        }}},
        {"f", {{
            std::vector<Position>{{0, -1}, {0, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{0, -1}, {0, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{1, -1}, {-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {1, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {-1, 1}}
        }}},
        {"g", {{
            std::vector<Position>{{0, -1}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}}
        }}},
        {"h", {{
            std::vector<Position>{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {-1, 0}, {0, 0}},
            std::vector<Position>{{0, -1}, {1, -1}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {1, -1}, {0, 0}, {1, 0}},
            std::vector<Position>{{-1, -1}, {0, -1}, {-1, 0}, {0, 0}}
        }}},
        {"i", {{
            std::vector<Position>{{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{0, 0}, {1, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {-1, 1}},
            std::vector<Position>{{0, -1}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {1, -1}, {-1, 0}, {0, 0}},
            std::vector<Position>{{1, -1}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {0, 1}}
        }}},
        {"j", {{
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}},
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}
        }}},
        {"k", {{
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}},
            std::vector<Position>{{1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {1, -1}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-1, 1}}
        }}},
        {"l", {{
            std::vector<Position>{{0, -2}, {0, -1}, {-1, 0}, {0, 0}, {-1, 1}},
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {1, 0}, {2, 0}},
            std::vector<Position>{{0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}},
            std::vector<Position>{{1, -1}, {0, 0}, {1, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{0, 0}, {1, 0}, {2, 0}, {-1, 1}, {0, 1}}
        }}},
        {"m", {{
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{0, -1}, {0, 0}, {1, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {1, -1}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}}
        }}},
        {"n", {{
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{0, -1}, {1, -1}, {0, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{-1, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}},
            std::vector<Position>{{0, -1}, {1, -1}, {0, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {1, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {1, 1}}
        }}},
        {"o", {{
            std::vector<Position>{{0, -1}, {0, 0}, {1, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{-1, 0}, {0, 0}, {1, 0}, {2, 0}, {0, 1}},
            std::vector<Position>{{0, -2}, {0, -1}, {-1, 0}, {0, 0}, {0, 1}},
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}},
            std::vector<Position>{{0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}
        }}},
        {"p", {{
            std::vector<Position>{{0, -1}, {0, 0}, {-1, 1}, {0, 1}, {1, 1}},
            std::vector<Position>{{0, -1}, {0, 0}, {-1, 1}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}},
            std::vector<Position>{{1, -1}, {-1, 0}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {1, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {1, -1}, {0, 0}, {0, 1}},
            std::vector<Position>{{1, -1}, {-1, 0}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}}
        }}},
        {"q", {{
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {1, 0}, {2, 0}},
            std::vector<Position>{{0, -2}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}},
            std::vector<Position>{{0, 0}, {1, 0}, {2, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{-2, 0}, {-1, 0}, {0, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{0, 0}, {1, 0}, {2, 0}, {0, 1}, {0, 2}},
            std::vector<Position>{{0, -2}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}},
            std::vector<Position>{{0, -2}, {0, -1}, {0, 0}, {1, 0}, {2, 0}}
        }}},
        {"r", {{
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {-1, 1}},
            std::vector<Position>{{1, -1}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{1, -1}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {-1, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {1, 0}, {1, 1}}
        }}},
        {"s", {{
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}},
            std::vector<Position>{{0, -1}, {1, -1}, {0, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {0, 1}, {1, 1}},
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}},
            std::vector<Position>{{0, -1}, {1, -1}, {0, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {0, 1}, {1, 1}}
        }}},
        {"t", {{
            std::vector<Position>{{-1, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{1, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {0, 1}},
            std::vector<Position>{{-1, -1}, {0, -1}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {1, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}},
            std::vector<Position>{{0, -1}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {0, 1}, {1, 1}}
        }}},
        {"u", {{
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}},
            std::vector<Position>{{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}}
        }}},
};
std::vector<std::string> BLOCK_IDS_BY_SIZE;

void init_block_ids_by_size() {
  BLOCK_IDS_BY_SIZE.clear();

  for (auto &[id, data] : block_table) {
    BLOCK_IDS_BY_SIZE.push_back(id);
  }

  std::sort(BLOCK_IDS_BY_SIZE.begin(), BLOCK_IDS_BY_SIZE.end(),
            [&](const std::string &a, const std::string &b) {
              return block_table.at(a).score > block_table.at(b).score;
            });
}

Block::Block(const BlockData &data) {
    rotations = data.rotations;
    cells = rotations[0];
    if (!cells.empty())
      return;

    shape.resize(5, vector<int>(5));
    for (int i = 0; i < 5; ++i)
      for (int j = 0; j < 5; ++j)
        shape[i][j] = data.shape[i][j];
    rebuild_occupied_offsets();
  }

vector<vector<int>> Block::transpose(const vector<vector<int>> &mat) {
    size_t rows = mat.size();
    size_t cols = mat[0].size();
    vector<vector<int>> res(cols, vector<int>(rows));
    for (size_t i = 0; i < rows; ++i)
      for (size_t j = 0; j < cols; ++j)
        res[j][i] = mat[i][j];
    return res;
  }

vector<vector<int>> Block::rot90(const vector<vector<int>> &mat, int k) {
    k = ((k % 4) + 4) % 4; // 安全に0〜3に正規化
    vector<vector<int>> res = mat;
    for (int i = 0; i < k; ++i) {
      size_t rows = res.size();
      size_t cols = res[0].size();
      vector<vector<int>> tmp(cols, vector<int>(rows));
      for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
          tmp[c][rows - 1 - r] = res[r][c];
      res = tmp;
    }
    return res;
  }

void Block::rebuild_occupied_offsets() {
  cells.clear();
  for (int r = 0; r < (int)shape.size(); ++r) {
    for (int c = 0; c < (int)shape[r].size(); ++c) {
      if (shape[r][c] == CANTSET) {
        cells.push_back({c - 2, r - 2});
      }
    }
  }
}

const vector<Position> &Block::occupied_offsets() const {
  return cells;
}

void Block::rotate_block(int dir) {
    int normalized_dir = ((dir % 8) + 8) % 8;
    if (!rotations[normalized_dir].empty())
      cells = rotations[normalized_dir];
    else
      rebuild_occupied_offsets();
  }

const BlockData &getBlock(const std::string &id) {
  static const std::unordered_map<std::string, BlockData> block_cache = [] {
    std::unordered_map<std::string, BlockData> cache;
    cache.reserve(block_table.size());
    for (const auto &[block_id, data] : block_table) {
      BlockData cached = data;
      auto rot_it = block_rotation_table.find(block_id);
      if (rot_it != block_rotation_table.end())
        cached.rotations = rot_it->second;
      cache.emplace(block_id, std::move(cached));
    }
    return cache;
  }();

  auto it = block_cache.find(id);
  if (it == block_cache.end()) {
    throw std::runtime_error("Error: block id '" + id + "' not found!");
  }
  return it->second;
}
