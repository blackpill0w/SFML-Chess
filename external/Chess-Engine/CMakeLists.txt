cmake_minimum_required(VERSION 3.10)
project(Chess-Engine CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic)

add_library(
  Chess-Engine SHARED
  piece.cpp
  king.cpp
  queen.cpp
  rook.cpp
  bishop.cpp
  knight.cpp
  pawn.cpp
  board_FEN.cpp
  board.cpp
)
