#ifndef PIECE_SPRITE_HPP
#define PIECE_SPRITE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Chess/board.hpp"
#include "Utils/utils.hpp"

using std::string;
using std::vector;

//********************************************
//**
//** A class to connect the pieces from
//** the 'Chess::Board' class and SFML sprites
//**
//********************************************

class PieceSprite {

public:
   unsigned pieceIndex;
   Chess::Board *board;
   bool alive;
   string pos;
   char pieceType;
protected:
   sf::Sprite sprite;
   const vector<sf::Texture> *textures;
   sf::RenderWindow *window;
public:
   PieceSprite(const vector<sf::Texture> *textures, const unsigned &pieceIndex, Chess::Board *board, sf::RenderWindow *window);

   PieceSprite(const PieceSprite& other);
   PieceSprite operator=(const PieceSprite& other) = delete;

   void changeTexture();
   void draw();
   void move(const sf::Vector2f &pos);
   bool isContainPos(const sf::Vector2f &pos) const;
   void update();
};

#endif
