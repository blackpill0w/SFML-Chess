#ifndef PIECE_SPRITE_HPP
#define PIECE_SPRITE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

#include "board.hpp"
#include "utils.hpp"

using std::string;
using std::vector;
using std::unique_ptr;

//********************************************
//**
//** A class to connect the pieces from
//** the 'Board' class and SFML sprites
//**
//********************************************

class PieceSprite {

public:
   sf::Sprite sprite;
   sf::RenderWindow *window;
   unsigned pieceIndex;
   Board *board;
   bool alive;
   string pos;
public:
   PieceSprite(sf::Texture &txtr, unsigned &pieceIndex, Board *board, sf::RenderWindow *window);

   PieceSprite(const PieceSprite& other);
   PieceSprite operator=(const PieceSprite& other) = delete;

   void move(const sf::Vector2f &pos);
   void draw();
   void update();

};

#endif