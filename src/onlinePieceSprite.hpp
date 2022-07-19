#ifndef ONLINE_SPRITE_HPP
#define ONLINE_SPRITE_HPP

#include <vector>
#include <SFML/Graphics.hpp>

#include "Utils/utils.hpp"

using std::vector;
using std::string;

class OnlinePieceSprite {
public:
  bool alive;
  char pieceType;
  string pos;
  vector< string > legalMoves;
  sf::Sprite sprite;
protected:
  sf::RenderWindow *window;
  const vector<sf::Texture> *textures;
public:
  OnlinePieceSprite(sf::RenderWindow *window, vector<sf::Texture> *textures);

  OnlinePieceSprite(const OnlinePieceSprite &other);
  OnlinePieceSprite operator=(const OnlinePieceSprite &other) = delete;

  void move(const sf::Vector2f &newPos);
  bool isContainPos(const sf::Vector2f &pos) const;
  bool isValidMove(string &move) const;
  void draw();
  void update(const bool &isAlive, const char type, const string &pos, const vector<string> &legalMoves);
protected:
  void changeTexture();
};

#endif
