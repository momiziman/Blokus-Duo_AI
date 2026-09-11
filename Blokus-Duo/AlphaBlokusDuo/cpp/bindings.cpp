#include "alpha_game.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using alphablokus::AlphaGameState;

PYBIND11_MODULE(_alpha_game, module) {
  module.doc() = "Blokus Duo game engine bridge for AlphaZero experiments";

  py::class_<Move>(module, "Move")
      .def(py::init<>())
      .def(py::init<std::string, int, int, int>())
      .def_readwrite("block_id", &Move::block_id)
      .def_readwrite("x", &Move::x)
      .def_readwrite("y", &Move::y)
      .def_readwrite("rotation", &Move::rotation)
      .def_property_readonly("is_pass", &Move::is_pass)
      .def("packed", &Move::packed)
      .def("__repr__", [](const Move &move) {
        if (move.is_pass())
          return std::string("Move(PASS)");
        return "Move(" + move.block_id + ", x=" + std::to_string(move.x) +
               ", y=" + std::to_string(move.y) +
               ", rotation=" + std::to_string(move.rotation) + ")";
      });

  py::class_<AlphaGameState>(module, "AlphaGameState")
      .def(py::init<>())
      .def("clone", [](const AlphaGameState &state) { return state; })
      .def("legal_moves", &AlphaGameState::legal_moves)
      .def("legal_actions", &AlphaGameState::legal_actions)
      .def("play", &AlphaGameState::play)
      .def("play_action", &AlphaGameState::play_action)
      .def_property_readonly("is_terminal", &AlphaGameState::is_terminal)
      .def_property_readonly("current_player",
                             &AlphaGameState::current_player)
      .def_property_readonly("consecutive_passes",
                             &AlphaGameState::consecutive_passes)
      .def_property_readonly("scores", &AlphaGameState::scores)
      .def_property_readonly("turns", &AlphaGameState::turns)
      .def_property_readonly("used_masks", &AlphaGameState::used_masks)
      .def("terminal_value", &AlphaGameState::terminal_value)
      .def("score_margin_value", &AlphaGameState::score_margin_value)
      .def("observation", &AlphaGameState::observation)
      .def("board_string", &AlphaGameState::board_string)
      .def("select_baseline", &AlphaGameState::select_baseline,
           py::arg("name"), py::arg("iterations") = 100,
           py::arg("max_tree_depth") = 10, py::arg("seed") = 0)
      .def_static("move_to_action", &AlphaGameState::move_to_action)
      .def_static("action_to_move", &AlphaGameState::action_to_move);

  module.attr("BOARD_SIZE") = alphablokus::kBoardSize;
  module.attr("PIECE_COUNT") = alphablokus::kPieceCount;
  module.attr("ORIENTATIONS") = alphablokus::kOrientations;
  module.attr("ACTION_SIZE") = alphablokus::kActionSize;
  module.attr("PASS_ACTION") = alphablokus::kPassAction;
  module.attr("OBSERVATION_CHANNELS") = alphablokus::kObservationChannels;
}
