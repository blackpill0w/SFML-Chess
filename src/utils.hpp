#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <limits>

#include "pieceSprite.hpp"
#include "board.hpp"

using std::string;
using std::vector;
using std::unique_ptr;

namespace utils {

   extern const unsigned int g_max_uint;
   extern const unsigned int fps;

   extern const int pieceSize;
   extern const int boardSize;
   extern const unsigned int wWidth;
   extern const unsigned int wHeight;
   extern const sf::String gameTitle;
   extern const sf::Vector2i wInitialPos;
   extern const string boardTexture;
   extern string textures[12];

   enum : unsigned int {wKing = 0u, bKing, wQueen, bQueen, wRook, bRook, wBishop, bBishop, wKnight, bKnight, wPawn, bPawn};

   sf::Vector2f adjustMousePos(const sf::Vector2f &mousePos);
   string posToStr(const sf::Vector2f &mousePos);
   sf::Vector2f strToVectorf(const string &pos);
   unsigned getSpriteIndexAt(const vector< unique_ptr<PieceSprite> > &sprites, const sf::Vector2f &pos);
   void highlightMoves(sf::RenderWindow *window, Board *board, unsigned pieceIndex);
   bool isThereSpriteAt(const vector<sf::Sprite> &sprites, const sf::Vector2f &pos);
   void checkDeadSprites(vector< unique_ptr<PieceSprite> > *sprites);
   void loadTextures(vector< sf::Texture > &textureList);
   unsigned getTextureIndex(char pieceType);
}

#endif