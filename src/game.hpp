#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "utils.hpp"
#include "Chess/board.hpp"

#include <string>
#include <vector>
#include <memory>
#include <random>
#include <iostream>

using std::cout;
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;

void startApp();

void playGame(sf::RenderWindow &window, const string &fenStr, const bool &vsHuman = true);

void loadSprites(
   vector< unique_ptr<PieceSprite> > &sprites,
   const vector< sf::Texture > &textures,
   Chess::Board &board,
   sf::RenderWindow &window
);

void keepPieceInsideBoard(sf::Vector2f &pos);

void draw(
   vector< unique_ptr<PieceSprite> > &sprites,
   unsigned &spritePressedIndex
);

void checkPromotion(
   sf::RenderWindow *window,
   vector< unique_ptr<PieceSprite> > &sprites,
   sf::Sprite &boardSprite,
   unsigned &spritePressedIndex,
   char &pieceToPromoteTo,
   string &to
);

void makeMove(
   Chess::Board &board,
   string &from,
   string &to,
   char &pieceToPromoteTo,
   unsigned &piecePressedIndex,
   vector< unique_ptr<PieceSprite> > &sprites,
   unsigned &spritePressedIndex
);

char getPieceToPromoteTo(
      sf::RenderWindow *window,
      sf::Sprite &boardSprite,
      vector< unique_ptr<PieceSprite> > *sprites,
      const unsigned &pieceToBePromotedIndex
);

int randomNumber(const int &from, const int &to);

void pickRandomMove(Chess::Board &board, string &from, string &to);

void playRandomMove(Chess::Board &board, string &from, string &to, vector<unique_ptr<PieceSprite>> &sprites);

void playSound(Chess::GameState &gameState, sf::Sound &sound);

#endif
