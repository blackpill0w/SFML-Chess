#include "game.hpp"

int main() {
   // Normal chess: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
   // For testing: "1r2kr2/pp1p1p2/2p4p/6pP/P1PP4/1P6/5PP1/R3K2R w KQ g6"
   sf::RenderWindow window(
      sf::VideoMode(utils::wWidth, utils::wHeight),
      utils::gameTitle,
      sf::Style::Titlebar | sf::Style::Close
   );
   window.setPosition( utils::wInitialPos );
   window.setFramerateLimit(utils::fps);
   playGame(window, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
   return 0;
}
