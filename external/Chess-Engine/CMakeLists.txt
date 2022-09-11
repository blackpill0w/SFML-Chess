cmake_minimum_required(VERSION 3.10)
project(Chess-Engine CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic)

add_library(
  Chess-Engine SHARED
  src/piece.cpp
  src/king.cpp
  src/queen.cpp
  src/rook.cpp
  src/bishop.cpp
  src/knight.cpp
  src/pawn.cpp
  src/board_FEN.cpp
  src/board.cpp
)
