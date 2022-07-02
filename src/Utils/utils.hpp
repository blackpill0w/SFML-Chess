#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

#include "../Chess/board.hpp"

#include "sfmlColors.hpp"
#include "button.hpp"

using std::string;
using std::vector;

namespace utils {

   //** Just a helper
   extern const unsigned int invalidIndex;
   //** FPS
   extern const unsigned int fps;

   //** Length of piece's side
   extern const int pieceSize;
   //** Length of the board (pSize * 8)
   extern const int boardSize;
   //** Width of window
   extern const unsigned int wWidth;
   //** Height of window
   extern const unsigned int wHeight;
   //** Game title
   extern const sf::String gameTitle;
   //** Initial position of window
   extern const sf::Vector2i wInitialPos;
   //** Font
   extern const string fontFile;
   //** Path to texture of the board
   extern const string boardTexture;
   //** Path to textures of the pieces
   extern string textures[12];
   //** To access the pieces' textures
   enum : unsigned int {wKing = 0u, bKing, wQueen, bQueen, wRook, bRook, wBishop, bBishop, wKnight, bKnight, wPawn, bPawn};
   //** Sound files
   extern string soundFiles[6];
   //** To access the sound files
   enum : unsigned int {
      gameStart = 0u, move,
      capture, check,
      castle, gameEnd
   };

   // Server settings
   extern const unsigned port;
   extern const string startGameStr;
   extern const string disconnectStr;

   //********************************************
   //**
   //** Given the mouse's position, it returns the top
   //** left point of the square containing
   //** the mouse (doesn't matter if there is a piece).
   //**
   //********************************************
   sf::Vector2f adjustMousePos(const sf::Vector2f &mousePos);

   //********************************************
   //**
   //** Converts an sf::Vector2f to a chess position
   //**
   //********************************************
   string posToStr(const sf::Vector2f &mousePos);

   //********************************************
   //**
   //** The opposite. A chess position to the top left
   //** corner of the corresponding square.
   //**
   //********************************************
   sf::Vector2f strToVectorf(const string &pos);

   //********************************************
   //**
   //** Highlights the moves of a piece.
   //**
   //********************************************
   void highlightMoves(sf::RenderWindow *window, vector<string> &moves);

   //********************************************
   //**
   //** Given a vector of textures, load the textures.
   //** Texture are assumed to be in this order:
   //**   wKing, bKing,
   //**   wQueen, bQueen,
   //**   wRook, bRook,
   //**   wBishop, bBishop,
   //**   wKnight, bKnight,
   //**   wPawn, bPawn
   //**
   //********************************************
   void loadTextures(vector< sf::Texture > &textureList);

   //********************************************
   //**
   //** Returns the index of the texture depending
   //** on the piece's (the 'Piece' class) type
   //**
   //********************************************
   unsigned getTextureIndex(const char pieceType);

   //********************************************
   //**
   //** Keeps a piece inside the board while being
   //** dragged.
   //**
   //********************************************
   void keepPieceInsideBoard(sf::Vector2f &pos);

   //********************************************
   //**
   //** Given a vector of PieceSprite/OnlinePieceSprite,
   //** return the index of the sprite at a given
   //** position (sf::Vector2f).
   //**
   //********************************************
   template<class T>
   unsigned getSpriteIndexAt(const vector< T > &sprites, const sf::Vector2f &pos) {
      unsigned index{ invalidIndex };
      for (unsigned j=0 ; j < sprites.size(); j++ ) {
         if (sprites[j].alive) {
            if (sprites[j].isContainPos(pos)) {
               index = j;
               break;
            }
         }
      }
      return index;
   };

   //********************************************
   //**
   //** Draws a vector of PieceSprite/OnlinePieceSprite.
   //**
   //********************************************
   template<class T>
   void drawSprites(vector<T> &sprites, const unsigned &spritePressedIndex = invalidIndex) {
      for (auto& sprite: sprites) {
         sprite.draw();
      }
      if (spritePressedIndex != invalidIndex) {
         sprites[spritePressedIndex].draw();
      }
   };
}

#endif
