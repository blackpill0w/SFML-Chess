#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "utils.hpp"
#include "board.hpp"
#include "pieceSprite.hpp"

int main() {
   sf::RenderWindow window(
      sf::VideoMode(utils::wWidth, utils::wHeight),
      utils::gameTitle,
      sf::Style::Titlebar | sf::Style::Close
   );
   window.setPosition( utils::wInitialPos );
   window.setFramerateLimit(utils::fps);

   // Board texture & sprite
   sf::Texture boardTexture;
   boardTexture.loadFromFile(utils::boardTexture);
   sf::Sprite board(boardTexture);
   board.setScale(utils::boardSize / board.getGlobalBounds().width, utils::boardSize / board.getGlobalBounds().height);

   Board b = Board();

   // Load textures (12 for 6 pieces & 2 colors)
   vector< sf::Texture > textures(12, sf::Texture());
   utils::loadTextures(textures);

   vector< unique_ptr<PieceSprite> > sprites;
   for (unsigned i=0u; i < b.pieces.size(); i++) {
      unsigned textureIndex{ 0u };
      textureIndex = utils::getTextureIndex(b.pieces[i]->type);
      PieceSprite temp(textures[textureIndex], i, &b, &window);
      sprites.emplace_back(make_unique<PieceSprite>(temp));
   }

   // Help with moving pieces
   sf::Vector2f mousePos;
   bool mouseReleased{ false };
   unsigned piecePressedIndex{ utils::g_max_uint };
   unsigned int spriteClickedIndex{ utils::g_max_uint };
   string from;
   string to;
   cout << "\n\n";
   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) {
            window.close();
         }
         else if (event.type == sf::Event::MouseButtonPressed) {
            spriteClickedIndex = utils::getSpriteIndexAt(sprites, mousePos);
            from = utils::posToStr(mousePos);
            piecePressedIndex = b.getIndexOfPieceAt(from);
         }
         else if (event.type == sf::Event::MouseButtonReleased) {
            mouseReleased = true;
         }
         else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
            cout << "spriteClickedIndex: "  <<  spriteClickedIndex << std::endl;
         }
      }
      window.clear();

            /****************************************/

      mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
      window.draw(board);

      if (spriteClickedIndex != utils::g_max_uint) {
         // Make sure the piece stays inside the board when moving it
         if (mousePos.x < (utils::pieceSize / 2))
            mousePos.x = (utils::pieceSize / 2);
         else if (mousePos.x > utils::boardSize - (utils::pieceSize / 2))
            mousePos.x = utils::boardSize - (utils::pieceSize / 2);
         if (mousePos.y < (utils::pieceSize / 2))
            mousePos.y = (utils::pieceSize / 2);
         else if (mousePos.y > utils::boardSize - (utils::pieceSize / 2))
            mousePos.y = utils::boardSize - (utils::pieceSize / 2);
         // Follow the mouse
         sprites[spriteClickedIndex]->move({mousePos.x - (utils::pieceSize / 2), mousePos.y - (utils::pieceSize / 2)});
         utils::highlightMoves(&window, &b, piecePressedIndex);
      }
      if (mouseReleased) {
         mouseReleased = false;
         if (spriteClickedIndex != utils::g_max_uint) {
            to = utils::posToStr(mousePos);
            // Tell the board to move (the moving piece is deduced from the var 'from')
            b.move(from, to);
            // sprites[spriteClickedIndex]->move( sf::Vector2f(-200, -200) );
            if ( b.pieces.size() != sprites.size() ) {
               piecePressedIndex = b.getIndexOfPieceAt(to);
            }
            // No need to check because the piece's position won't change
            // if the move is invalid
            sprites[spriteClickedIndex]->move( utils::strToVectorf(b.pieces[piecePressedIndex]->pos) );
            spriteClickedIndex = utils::g_max_uint;
            for (auto& sprite: sprites) {
               sprite->update();
            }
         }
      }
      for (auto& sprite: sprites) {
         sprite->draw();
      }
      if (spriteClickedIndex != utils::g_max_uint) {
         sprites[spriteClickedIndex]->draw();
         utils::checkDeadSprites(&sprites);
      }
      b.update();

            /****************************************/

      window.display();
   }
}
