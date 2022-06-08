#ifndef PIECE_SPRITE_HPP
#define PIECE_SPRITE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

#include "board.hpp"

using std::string;
using std::vector;
using std::unique_ptr;

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
   static sf::Vector2f strToVector(const std::string &pos);
   static std::string posToStr(const sf::Vector2f &mousePos);
   void move(const sf::Vector2f &pos);
   void draw();
   void update();

};

#endif