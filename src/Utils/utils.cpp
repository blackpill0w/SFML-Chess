#include "utils.hpp"

// File seperators
const string sep{ "/" };
#ifdef _WIN32
   sep = "\\"
#endif

namespace utils {

   extern const unsigned int invalidIndex{ 5000u }; // Big enough for a chess game
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

   // Server settings
   extern const unsigned port{ 9991u };
   extern const string startGameStr( "*start game*" );
   extern const string disconnectStr( "*disconnect*" );

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

   void highlightMoves(sf::RenderWindow *window, vector<string> &moves) {
      for (auto& move: moves) {
         unique_ptr< sf::RectangleShape > p{ make_unique< sf::RectangleShape >(sf::RectangleShape({70, 70})) };
         p->setPosition(strToVectorf(move));
         p->setFillColor(sf::Color(255, 0, 0, 120));
         window->draw(*p);
      }
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

      textureList.clear();
      textureList.reserve(12);

      for (unsigned i = 0u; i < 12u; i++) {
         textureList.emplace_back( sf::Texture() );
         textureList[i].loadFromFile(textures[i]);
      }
   }

   unsigned getTextureIndex(const char pieceType) {
      unsigned int index{ invalidIndex };
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

   void keepPieceInsideBoard(sf::Vector2f &pos) {
      if (pos.x < (pieceSize / 2.f))
         pos.x = (pieceSize / 2.f);
      else if (pos.x > boardSize - (pieceSize / 2.f))
         pos.x = boardSize - (pieceSize / 2.f);
      if (pos.y < (pieceSize / 2.f))
         pos.y = (pieceSize / 2.f);
      else if (pos.y > boardSize - (pieceSize / 2.f))
         pos.y = boardSize - (pieceSize / 2.f);
   }


} // utils namespace
