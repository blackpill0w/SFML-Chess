#include "game.hpp"

void startApp() {
   sf::RenderWindow window(
      sf::VideoMode(utils::wWidth, utils::wHeight),
      utils::gameTitle,
      sf::Style::Titlebar | sf::Style::Close
   );
   window.setPosition( utils::wInitialPos );
   window.setFramerateLimit(utils::fps);

   // Normal chess: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
   // For testing: "1r2kr2/pp1p1p2/2p4p/6pP/P1PP4/1P6/5PP1/R3K2R w KQ g6"
   playGame(window, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}

void playGame(sf::RenderWindow &window, const string &fenStr, const bool &vsHuman) {
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
      (utils::wWidth / 2) - gameEndText.getGlobalBounds().width/2,
      (utils::wHeight / 2) - gameEndText.getGlobalBounds().height/2  - 15
   ));

   sf::RectangleShape gameEndRect(sf::Vector2f(200.f, 100.f));
   gameEndRect.setFillColor(sf::Color(0, 0, 0, 150));
   gameEndRect.setPosition(sf::Vector2f(
      (utils::wWidth / 2) - gameEndRect.getGlobalBounds().width/2,
      (utils::wHeight / 2) - gameEndRect.getGlobalBounds().height/2
   ));

   Chess::Board board(fenStr);

   // Load textures (12 for 6 pieces & 2 colors)
   vector< sf::Texture > textures(12, sf::Texture());
   utils::loadTextures(textures);

   // Sprites of the pieces
   vector< unique_ptr<PieceSprite> > sprites{};
   loadSprites(sprites, textures, board, window);

   // Helpers
   sf::Vector2f mousePos{};
   bool mouseReleased{ false };
   //** This keeps track of the sprite pressed (being moved)
   unsigned int spritePressedIndex{ utils::invalidIndex };
   //** And this is the corresponding piece (from Board)
   unsigned piecePressedIndex{ utils::invalidIndex };
   // From and to where the sprite has moved
   string from{};
   string to{};
   char pieceToPromoteTo{ 0 };

   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) {
            window.close();
         }
         else if (event.type == sf::Event::MouseButtonPressed) {
            spritePressedIndex = utils::getSpriteIndexAt(sprites, mousePos);
            if (spritePressedIndex != utils::invalidIndex) {
               piecePressedIndex = sprites[spritePressedIndex]->pieceIndex;
               from = utils::posToStr(mousePos);
            }
         }
         else if (event.type == sf::Event::MouseButtonReleased) {
            mouseReleased = true;
         }
         else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {
            board.undoLastMove();
            for (auto& sprite: sprites) {
               sprite->update();
            }
         }
      }
      window.clear();

            /****************************************/

      mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
      window.draw(boardSprite);
      if (spritePressedIndex != utils::invalidIndex) {
         // Make sure the piece stays inside the board when moving it
         keepPieceInsideBoard(mousePos);
         // Follow the mouse
         sprites[spritePressedIndex]->move({mousePos.x - (utils::pieceSize / 2), mousePos.y - (utils::pieceSize / 2)});
         if (board.pieces[piecePressedIndex]->color == board.turn) {
            utils::highlightMoves(&window, &board, piecePressedIndex);
         }
      }
      if (mouseReleased) {
         mouseReleased = false;
         if (spritePressedIndex != utils::invalidIndex) {
            to = utils::posToStr(mousePos);
            if (spritePressedIndex != utils::invalidIndex && board.pieces[piecePressedIndex]->isValidMove(to)) {
               checkPromotion(&window, sprites, boardSprite, spritePressedIndex, pieceToPromoteTo, to);
            }
            makeMove(board, from, to, pieceToPromoteTo, piecePressedIndex, sprites, spritePressedIndex);
            playSound(board.gameState, sound);
            if (!vsHuman && board.gameState == Chess::Moved  && board.turn == Chess::BLACK) {
               playRandomMove(board, from, to, sprites);
            }
            if (board.gameState == Chess::Draw) {
               gameEndText.setString("Draw");
            }
         }
      }
      draw(sprites, spritePressedIndex);
      if (board.gameState == Chess::Draw || board.gameState == Chess::CheckMate) {
         window.draw(gameEndRect);
         window.draw(gameEndText);
      }

            /****************************************/

      window.display();
   }
}

void keepPieceInsideBoard(sf::Vector2f &pos) {
   if (pos.x < (utils::pieceSize / 2))
      pos.x = (utils::pieceSize / 2);
   else if (pos.x > utils::boardSize - (utils::pieceSize / 2))
      pos.x = utils::boardSize - (utils::pieceSize / 2);
   if (pos.y < (utils::pieceSize / 2))
      pos.y = (utils::pieceSize / 2);
   else if (pos.y > utils::boardSize - (utils::pieceSize / 2))
      pos.y = utils::boardSize - (utils::pieceSize / 2);
}

void draw(
   vector< unique_ptr<PieceSprite> > &sprites,
   unsigned &spritePressedIndex
) {
   for (auto& sprite: sprites) {
      sprite->draw();
   }
   // Keep selected sprite on top of all sprites
   if (spritePressedIndex != utils::invalidIndex) {
      sprites[spritePressedIndex]->draw();
   }
}

void loadSprites(
   vector< unique_ptr<PieceSprite> > &sprites,
   const vector< sf::Texture > &textures,
   Chess::Board &board,
   sf::RenderWindow &window
) {
   for (unsigned i=0u; i < board.pieces.size(); i++) {
      PieceSprite temp(&textures, i, &board, &window);
      sprites.emplace_back(make_unique<PieceSprite>(temp));
   }
}

void makeMove(
   Chess::Board &board,
   string &from,
   string &to,
   char &pieceToPromoteTo,
   unsigned &piecePressedIndex,
   vector< unique_ptr<PieceSprite> > &sprites,
   unsigned &spritePressedIndex
) {
   // Tell the board to move (the moving piece is deduced from the variable 'from')
   board.move(from, to, pieceToPromoteTo);

   // Move the sprite to the new position, if the move isn't valid the piece (from Board) won't
   // move, meaning the sprite goes back to its initial position, so, no need for checking.
   sprites[spritePressedIndex]->move( utils::strToVectorf(board.pieces[piecePressedIndex]->pos) );
   spritePressedIndex = utils::invalidIndex;
   for (auto& sprite: sprites) {
      sprite->update();
   }
}

void checkPromotion(
   sf::RenderWindow *window,
   vector< unique_ptr<PieceSprite> > &sprites,
   sf::Sprite &boardSprite,
   unsigned &spritePressedIndex,
   char &pieceToPromoteTo,
   string &to
) {
   if ( tolower(sprites[spritePressedIndex]->pieceType) == 'p' && (to[1] == '1' || to[1] == '8')) {
      // Get the type of piece to promote to
      pieceToPromoteTo = getPieceToPromoteTo(window, boardSprite, &sprites, spritePressedIndex);
   }
}

char getPieceToPromoteTo(
      sf::RenderWindow *window,
      sf::Sprite &boardSprite,
      vector< unique_ptr<PieceSprite> > *sprites,
      const unsigned &pieceToBePromotedIndex
) {
   char piece{ 0 };
   // Get mouse's position
   sf::Vector2f mousePos;

   sf::Vector2f rectSize(utils::boardSize - 200.f, 100.f);

   sf::RectangleShape rects[4] = {
      sf::RectangleShape(rectSize),
      sf::RectangleShape(rectSize),
      sf::RectangleShape(rectSize),
      sf::RectangleShape(rectSize)
   };
   sf::Font font;
   font.loadFromFile(utils::fontFile);
   sf::Text texts[4] = {
      sf::Text("Queen", font),
      sf::Text("Rook", font),
      sf::Text("Bishop", font),
      sf::Text("Knight", font)
   };

   for (int i=0; i<4; i++) {
      sf::Vector2f pos(100.f, i*110.f + 80.f);
      rects[i].setFillColor(sf::Color(0u, 0u, 0u, 255u));
      rects[i].setPosition(pos);
      texts[i].setFillColor(sf::Color(255u, 255u, 255u, 255u));
      pos.x += (rects[i].getSize().x / 2.f) - (texts[i].getGlobalBounds().width / 2.f);
      pos.y += 30.f;
      texts[i].setPosition(pos);
   }

   while (piece == 0 && window->isOpen()) {
      sf::Event event;

      while (window->pollEvent(event)) {
         if (event.type == sf::Event::Closed) {
            window->close();
            return 0;
         }
         if (event.type == sf::Event::MouseButtonPressed) {
            for (int i=0; i < 4; i++) {
               if ( rects[i].getGlobalBounds().contains( mousePos )) {
                  switch (i) {
                     case 0:
                        piece = 'q';
                        break;
                     case 1:
                        piece = 'r';
                        break;
                     case 2:
                        piece = 'b';
                        break;
                     case 3:
                        piece = 'n';
                        break;
                  }
                  // Break from the for loop
                  break;
               }
            }
         }
      }

      // Get mouse's position
      mousePos = sf::Vector2f( sf::Mouse::getPosition(*window) );

      window->clear();

      for (int i=0; i < 4; i++) {
         if ( rects[i].getGlobalBounds().contains( mousePos )) {
            rects[i].setFillColor(sf::Color(45u, 25u, 52u, 255u));
         }
         else {
            rects[i].setFillColor(sf::Color(0u, 0u, 0u, 255u));
         }
      }

      window->draw(boardSprite);
      // Draw pieces
      for (auto& s: *sprites) {
         s->draw();
      }
      // Draw promotion menu
      for (int i=0; i<4; i++) {
         window->draw(rects[i]);
         window->draw(texts[i]);
      }

      window->display();
   }
   // Match cas of the choice with the piece (make them have the same color)
   if ( isupper( (*sprites)[pieceToBePromotedIndex]->pieceType ) ) {
      piece = toupper(piece);
   }
   return piece;
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

void playRandomMove(Chess::Board &board, string &from, string &to, vector<unique_ptr<PieceSprite>> &sprites) {
   pickRandomMove(board, from, to);
   board.move(from, to, 'q');
   for (auto& sprite: sprites) {
      sprite->update();
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

