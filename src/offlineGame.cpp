#include "offlineGame.hpp"

void offlineGame(sf::RenderWindow &window, const string &fenStr) {
   // Sound
   sf::Sound sound;

   // Board texture
   sf::Texture boardTexture{};
   boardTexture.loadFromFile(utils::boardTexture);
   sf::Sprite boardSprite(boardTexture);
   boardSprite.setScale(
      utils::boardSize / boardSprite.getGlobalBounds().width,
      utils::boardSize / boardSprite.getGlobalBounds().height
   );

   // Font and texts
   sf::Font font;
   font.loadFromFile("assets/fonts/UbuntuMono-R.ttf");

   sf::Text gameEndText("Checkmate", font);
   gameEndText.setPosition(sf::Vector2f(
      (utils::wWidth / 2.f) - gameEndText.getGlobalBounds().width/2,
      (utils::wHeight / 2.f) - gameEndText.getGlobalBounds().height/2  - 15
   ));

   sf::RectangleShape gameEndRect(sf::Vector2f(200.f, 100.f));
   gameEndRect.setFillColor(sf::Color(0, 0, 0, 150));
   gameEndRect.setPosition(sf::Vector2f(
      (utils::wWidth / 2.f) - gameEndRect.getGlobalBounds().width/2,
      (utils::wHeight / 2.f) - gameEndRect.getGlobalBounds().height/2
   ));

   Chess::Board board(fenStr);

   // Load textures (12 for 6 pieces & 2 colors)
   vector< sf::Texture > textures;
   utils::loadTextures(textures);

   // Sprites of the pieces
   vector< PieceSprite > sprites{};
   loadSprites(sprites, textures, board, window);

   // Helpers
   sf::Vector2f   mousePos{};
   bool           mouseReleased{ false };
   //** This keeps track of the sprite being dragged
   unsigned       spritePressedIndex{ utils::invalidIndex };
   //** And this is the corresponding piece (from Board)
   unsigned       piecePressedIndex{ utils::invalidIndex };
   // From and to where the sprite has moved
   string         from{};
   string         to{};
   char           pieceToPromoteTo{ 'q' };

   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) {
            window.close();
         }
         else if (event.type == sf::Event::MouseButtonPressed) {
            spritePressedIndex = utils::getSpriteIndexAt(sprites, mousePos);
            if (spritePressedIndex != utils::invalidIndex) {
               piecePressedIndex = sprites[spritePressedIndex].pieceIndex;
               from = utils::posToStr(mousePos);
            }
         }
         else if (event.type == sf::Event::MouseButtonReleased) {
            if (spritePressedIndex != utils::invalidIndex) {
               mouseReleased = true;
            }
         }
         else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {
            board.undoLastMove();
            for (auto& sprite: sprites) {
               sprite.update();
            }
         }
         else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
            if (board.gameState == Chess::Draw || board.gameState == Chess::CheckMate) {
               board.reset();
               for (auto& sprite: sprites) {
                  sprite.update();
               }
            }
         }
      }
      window.clear();

            /****************************************/

      mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
      window.draw(boardSprite);
      if (spritePressedIndex != utils::invalidIndex) {
         // Make sure the piece stays inside the board when moving it
         utils::keepPieceInsideBoard(mousePos);
         // Follow the mouse
         sprites[spritePressedIndex].move({mousePos.x - (utils::pieceSize / 2.f), mousePos.y - (utils::pieceSize / 2.f)});
         if (board.pieces[piecePressedIndex]->color == board.turn) {
            utils::highlightMoves(&window, board.pieces[spritePressedIndex]->legalMoves);
         }
         // Play move
         if (mouseReleased) {
            mouseReleased = false;
            to = utils::posToStr(mousePos);
            if (board.pieces[piecePressedIndex]->isValidMove(to)) {
              pieceToPromoteTo = utils::checkPromotion(&window, sprites, boardSprite, spritePressedIndex, to);
            }
            makeMove(board, from, to, pieceToPromoteTo, piecePressedIndex, sprites, spritePressedIndex);
            playSound(board.gameState, sound);
            //if (!vsHuman && board.gameState == Chess::Moved  && board.turn == Chess::BLACK) {
            //   playRandomMove(board, from, to, sprites);
            //}
            if (board.gameState == Chess::Draw) {
               gameEndText.setString("Draw");
            }
         }
      }
      utils::drawSprites(sprites, spritePressedIndex);
      if (board.gameState == Chess::Draw || board.gameState == Chess::CheckMate) {
         window.draw(gameEndRect);
         window.draw(gameEndText);
      }

            /****************************************/

      window.display();
   }

}

void loadSprites(
   vector< PieceSprite > &sprites,
   const vector< sf::Texture > &textures,
   Chess::Board &board,
   sf::RenderWindow &window
) {
   sprites.reserve(board.pieces.size());
   for (unsigned i=0u; i < board.pieces.size(); i++) {
      sprites.emplace_back( PieceSprite(&textures, i, &board, &window) );
   }
}

void makeMove(
   Chess::Board &board,
   string &from,
   string &to,
   char pieceToPromoteTo,
   unsigned &piecePressedIndex,
   vector< PieceSprite > &sprites,
   unsigned &spritePressedIndex
) {
   // Make a move
   board.move(from, to, pieceToPromoteTo);

   // Move the sprite to the new position, if the move isn't valid the piece (from Board) won't
   // move, meaning the sprite goes back to its initial position, so, no need for checking.
   sprites[spritePressedIndex].move( utils::strToVectorf(board.pieces[piecePressedIndex]->pos) );
   spritePressedIndex = utils::invalidIndex;
   for (auto& sprite: sprites) {
      sprite.update();
   }
}

int randomNumber(const int &from, const int &to) {
   std::random_device dev;
   std::mt19937 rng(dev());
   std::uniform_int_distribution<std::mt19937::result_type> randomNum(static_cast<unsigned>(from), static_cast<unsigned>(to));
   return randomNum(rng);
}

void pickRandomMove(Chess::Board &board, string &from, string &to) {
   vector<string> piecesToMove{};
   for (auto& piece: board.pieces) {
      if (piece->color == board.turn && piece->alive) {
         piecesToMove.emplace_back(piece->pos);
      }
   }

   // Picking a random piece
   unsigned pieceIndex{ 0u };
   do {
      from = piecesToMove[ static_cast<unsigned>(randomNumber(0, piecesToMove.size() - 1)) ];
      pieceIndex = board.getIndexOfPieceAt(from);
   } while (board.pieces[pieceIndex]->legalMoves.size() == 0u);

   // Picking random move
   to = board.pieces[pieceIndex]->legalMoves[ static_cast<unsigned>(randomNumber(0, board.pieces[pieceIndex]->legalMoves.size() - 1))];
}

void playRandomMove(Chess::Board &board, string &from, string &to, vector<PieceSprite> &sprites) {
   pickRandomMove(board, from, to);
   board.move(from, to, 'q');
   for (auto& sprite: sprites) {
      sprite.update();
   }
}

void playSound(Chess::GameState &gameState, sf::Sound &sound) {
   static sf::SoundBuffer gameStart;
   gameStart.loadFromFile(utils::soundFiles[utils::gameStart]);
   static sf::SoundBuffer move;
   move.loadFromFile(utils::soundFiles[utils::move]);
   static sf::SoundBuffer capture;
   capture.loadFromFile(utils::soundFiles[utils::capture]);
   static sf::SoundBuffer check;
   check.loadFromFile(utils::soundFiles[utils::check]);
   static sf::SoundBuffer castle;
   castle.loadFromFile(utils::soundFiles[utils::castle]);
   static sf::SoundBuffer gameEnd;
   gameEnd.loadFromFile(utils::soundFiles[utils::gameEnd]);

   switch (gameState) {
      case Chess::Moved:
         sound.setBuffer(move);
         break;
      case Chess::MoveAndCapture:
         sound.setBuffer(capture);
         break;
      case Chess::MoveAndCastle:
         sound.setBuffer(castle);
         break;
      case Chess::Check:
         sound.setBuffer(check);
         break;
      case Chess::CheckMate:
      case Chess::Draw:
         sound.setBuffer(gameEnd);
         break;
      default:
         break;
   }
   if (gameState != Chess::MoveFailed) {
      sound.play();
   }
}
