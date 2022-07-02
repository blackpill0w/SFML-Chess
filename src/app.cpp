#include "app.hpp"

void startApp() {
   sf::RenderWindow window(
      sf::VideoMode(utils::wWidth, utils::wHeight),
      utils::gameTitle,
      sf::Style::Titlebar | sf::Style::Close
   );
   window.setPosition( utils::wInitialPos );
   window.setFramerateLimit(utils::fps);

   sf::Font font;
   font.loadFromFile(utils::fontFile);

   // Normal chess: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
   // For testing: "1r2kr2/pp1p1p2/2p4p/6pP/P1PP4/1P6/5PP1/R3K2R w KQ g6"
   const std::function<void()> offline( [&]() { offlineGame(window, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");} );
   const std::function<void()> online( [&]() { onlineGame(window);} );

   utils::Button offlineGameButton(sf::Vector2f(200.f, 200.f), "Offline", &font, offline);
   utils::Button onlineGameButton(sf::Vector2f(200.f, 250.f), "Online", &font, online);

   bool mouseClicked{ false };
   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) {
            window.close();
         }
         else if (event.type == sf::Event::MouseButtonPressed) {
            mouseClicked = true;
         }
      }
      window.clear(sf::Color(80, 00, 80));

      sf::Vector2f mousePos( sf::Mouse::getPosition(window) );
      onlineGameButton.update(window, mousePos, mouseClicked);
      offlineGameButton.update(window, mousePos, mouseClicked);
      if (mouseClicked) {
         mouseClicked = false;
      }

      window.display();
   }
}
