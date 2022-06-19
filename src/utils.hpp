#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <limits>

#include "Chess/board.hpp"

#include "pieceSprite.hpp"
class PieceSprite;

using std::string;
using std::vector;
using std::unique_ptr;

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
   //** Given a vector of (unique_ptr to) 'PieceSprite's,
   //** return the index of the sprite at a given
   //** position (sf::Vector2f).
   //**
   //********************************************
   unsigned getSpriteIndexAt(const vector< unique_ptr<PieceSprite> > &sprites, const sf::Vector2f &pos);

   //********************************************
   //**
   //** Highlights the moves of a piece.
   //**
   //********************************************
   void highlightMoves(sf::RenderWindow *window, Chess::Board *board, unsigned &pieceIndex);

   //********************************************
   //**
   //** Checks wether there is a sprite at
   //** a given position.
   //**
   //********************************************
   bool isThereSpriteAt(const vector<sf::Sprite> &sprites, const sf::Vector2f &pos);

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
   unsigned getTextureIndex(const char &pieceType);
}

#endif
