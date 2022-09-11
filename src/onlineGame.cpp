#include "./onlineGame.hpp"
#include "./piece.hpp"

using std::string;
using std::vector;

void onlineGame(sf::RenderWindow &window) {
   sf::TcpSocket client;

   client.connect("localhost", utils::port);

   sf::Packet dataPacket;
   Chess::PieceColor myColor{ Chess::WHITE };
   string color;
   client.receive(dataPacket);
   dataPacket >> color;
   if (color == "black") {
      myColor = Chess::BLACK;
   }
   dataPacket.clear();
   client.setBlocking(false);

   vector<sf::Texture> textures;
   utils::loadTextures(textures);

   vector<OnlinePieceSprite> sprites(32, OnlinePieceSprite(&window, &textures));

   sf::Texture boardTexture{};
   boardTexture.loadFromFile(utils::boardTexture);
   sf::Sprite boardSprite(boardTexture);
   boardSprite.setScale(
      utils::boardSize / boardSprite.getGlobalBounds().width,
      utils::boardSize / boardSprite.getGlobalBounds().height
   );

   sf::Vector2f   mousePos{};
   string         from{};
   string         to{};
   char           pieceToPromoteTo{};
   bool           mouseReleased{ false };
   unsigned       spritePressedIndex{ utils::invalidIndex };
   GameData       gameData;

   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) {
            client.disconnect();
            window.close();
         }
         else if (event.type == sf::Event::MouseButtonPressed) {
            spritePressedIndex = utils::getSpriteIndexAt(sprites, mousePos);
            if (spritePressedIndex != utils::invalidIndex) {
               from = utils::posToStr(mousePos);
            }
         }
         else if (event.type == sf::Event::MouseButtonReleased) {
            if (gameData.turn == myColor) {
               mouseReleased = true;
            }
            else if (spritePressedIndex != utils::invalidIndex) {
               sprites[spritePressedIndex].move( utils::strToVectorf(sprites[spritePressedIndex].pos) );
               spritePressedIndex = utils::invalidIndex;
            }
         }
      }
      window.clear();

         /***********************************/

      window.draw(boardSprite);

      checkBoardUpdate(client, sprites, dataPacket, gameData);

      mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

      if (spritePressedIndex != utils::invalidIndex) {
         utils::keepPieceInsideBoard(mousePos);
         sprites[spritePressedIndex].move({mousePos.x - (utils::pieceSize / 2.f), mousePos.y - (utils::pieceSize / 2.f)});

         if (gameData.turn == myColor) {
            utils::highlightMoves(&window, sprites[spritePressedIndex].legalMoves);
         }
         if (mouseReleased) {
            mouseReleased = false;
            to = utils::posToStr(mousePos);

            if (sprites[spritePressedIndex].isValidMove(to)) {
              pieceToPromoteTo = utils::checkPromotion(&window, sprites, boardSprite, spritePressedIndex, to);
            }

            sendMove(client, from, to, pieceToPromoteTo, dataPacket);
            spritePressedIndex = utils::invalidIndex;
         }
      }
      utils::drawSprites(sprites, spritePressedIndex);

         /***********************************/

      window.display();
   }
}

void updateSprites(const GameData &gameData, vector<OnlinePieceSprite> &sprites) {
   for (unsigned i=0u; i < sprites.size(); i++) {
      sprites[i].update(gameData.piecesData[i].alive, gameData.piecesData[i].type, gameData.piecesData[i].pos, gameData.piecesData[i].legalMoves);
   }
}

void checkBoardUpdate(sf::TcpSocket &client, vector<OnlinePieceSprite> &sprites, sf::Packet &dataPacket, GameData &gameData) {
   client.receive(dataPacket);
   GameData temp;
   dataPacket >> temp;
   if (temp.piecesData.size() != 0) {
      updateSprites(temp, sprites);
      gameData = temp;
   }
   dataPacket.clear();
}

void sendMove(sf::TcpSocket &client, const string &from, const string &to, const char pieceToPromoteTo, sf::Packet &dataPacket) {
   dataPacket << from << to << static_cast<int>(pieceToPromoteTo);
   client.send(dataPacket);
   dataPacket.clear();
}
