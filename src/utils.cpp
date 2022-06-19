#include "utils.hpp"

// File seperators
const string sep{ "/" };
#ifdef _WIN32
   sep = "\\"
#endif

namespace utils {

   extern const unsigned int invalidIndex{ std::numeric_limits< unsigned int >::max() };
   extern const unsigned int fps{ 60u };

   extern const int pieceSize{ 70 };
   extern const int boardSize{ 8*pieceSize };
   extern const unsigned int wWidth{ static_cast< unsigned >(boardSize) };
   extern const unsigned int wHeight{ static_cast< unsigned >(boardSize) };
   extern const sf::String gameTitle{ "Chess" };
   extern const sf::Vector2i wInitialPos(100, 100);
   extern const string fontFile{ "assets/fonts/UbuntuMono-R.ttf" };
   extern const string boardTexture{ "assets/img/chess-board.png" };

   string texturesFolder{ "assets" + sep + "img" + sep + "pieces" + sep };
   string textures[12] = {
      texturesFolder + "wk.png",
      texturesFolder + "bk.png",
      texturesFolder + "wq.png",
      texturesFolder + "bq.png",
      texturesFolder + "wr.png",
      texturesFolder + "br.png",
      texturesFolder + "wb.png",
      texturesFolder + "bb.png",
      texturesFolder + "wn.png",
      texturesFolder + "bn.png",
      texturesFolder + "wp.png",
      texturesFolder + "bp.png"
   };

   string soundFilesFolder{ "assets" + sep + "sound_effects" + sep };
   string soundFiles[6] = {
      soundFilesFolder + "game-start.ogg",
      soundFilesFolder + "move.ogg",
      soundFilesFolder + "capture.ogg",
      soundFilesFolder + "check.ogg",
      soundFilesFolder + "castle.ogg",
      soundFilesFolder + "game-end.ogg",
   };


   sf::Vector2f adjustMousePos(const sf::Vector2f &mousePos) {
      sf::Vector2f m;
      m.x = static_cast<int>(mousePos.x/pieceSize) * pieceSize;
      m.y = static_cast<int>(mousePos.y/pieceSize) * pieceSize;
      return m;
   }

   string posToStr(const sf::Vector2f &mousePos) {
      string p;
      p += ('a' + static_cast<int>(mousePos.x/pieceSize));
      p += ('1' + (8 - 1 - static_cast<int>(mousePos.y/pieceSize)));
      return p;
   }

   sf::Vector2f strToVectorf(const string &pos) {
      sf::Vector2f p;
      p.x = (pos[0] - 'a') * pieceSize;
      p.y = ( 7 - (pos[1] - '1')) * pieceSize;
      return p;
   }

   unsigned getSpriteIndexAt(const vector< unique_ptr<PieceSprite> > &sprites, const sf::Vector2f &pos) {
      unsigned index{ invalidIndex };
      for (unsigned j=0 ; j < sprites.size(); j++ ) {
         if (sprites[j]->alive) {
            if (sprites[j]->sprite.getGlobalBounds().contains(pos)) {
               index = j;
               break;
            }
         }
      }
      return index;
   }

   void highlightMoves(sf::RenderWindow *window, Chess::Board *board, unsigned &pieceIndex) {
      for (auto& pos: board->pieces[pieceIndex]->legalMoves) {
         unique_ptr< sf::RectangleShape > p{ make_unique< sf::RectangleShape >(sf::RectangleShape({70, 70})) };
         p->setPosition(strToVectorf(pos));
         p->setFillColor(sf::Color(255, 0, 0, 120));
         window->draw(*p);
      }
   }

   bool isThereSpriteAt(const vector<sf::Sprite> &sprites, const sf::Vector2f &pos) {
      for (auto& sprite: sprites) {
         if (sprite.getGlobalBounds().contains(pos)) {
            return true;
         }
      }
      return false;
   }

   void loadTextures(vector< sf::Texture > &textureList) {
      //**
      //**********************************
      //**
      //** Texture are assumed to be in this order:
      //**   wKing, bKing,
      //**   wQueen, bQueen,
      //**   wRook, bRook,
      //**   wBishop, bBishop,
      //**   wKnight, bKnight,
      //**   wPawn, bPawn
      //**
      //**********************************

      for (unsigned i = 0u; i < 12u; i++) {
         textureList[i].loadFromFile(textures[i]);
      }
   }

   unsigned getTextureIndex(const char &pieceType) {
      unsigned int index{ 0u };
      switch (pieceType){
         case 'K':
            index = wKing;
            break;
         case 'k':
            index = bKing;
            break;
         case 'Q':
            index = wQueen;
            break;
         case 'q':
            index = bQueen;
            break;
         case 'R':
            index = wRook;
            break;
         case 'r':
            index = bRook;
            break;
         case 'B':
            index = wBishop;
            break;
         case 'b':
            index = bBishop;
            break;
         case 'N':
            index = wKnight;
            break;
         case 'n':
            index = bKnight;
            break;
         case 'P':
            index = wPawn;
            break;
         case 'p':
            index = bPawn;
            break;
         default:
            break;
      }
      return index;
   }
}
