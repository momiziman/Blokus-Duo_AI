cmake_minimum_required(VERSION 3.20)
project(AlphaBlokusDuo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_alpha_game
  cpp/alpha_game.cpp
  cpp/bindings.cpp
  ../src/board.cpp
  ../src/piece.cpp
  ../src/game.cpp
)

target_include_directories(_alpha_game PRIVATE ../include cpp)
