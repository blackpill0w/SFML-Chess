cmake_minimum_required(VERSION 3.10)

project(SFML-Chess CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic)

add_executable(server
  src/server.cpp
  src/utils.cpp
  src/gameData.cpp
)

add_executable(app
  src/main.cpp
  src/app.cpp
  src/offlineGame.cpp
  src/onlineGame.cpp
  src/utils.cpp
  src/button.cpp
  src/sfmlColors.cpp
  src/pieceSprite.cpp
  src/onlinePieceSprite.cpp
  src/gameData.cpp
)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(server sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(app sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

target_include_directories(server PUBLIC external/Chess-Engine)
target_include_directories(app PUBLIC external/Chess-Engine)

add_subdirectory(external/Chess-Engine)
target_link_libraries(server Chess-Engine)
target_link_libraries(app Chess-Engine)
