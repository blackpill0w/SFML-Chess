#ifndef OFFLINE_GAME_HPP
#define OFFLINE_GAME_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Utils/utils.hpp"
#include "pieceSprite.hpp"
#include "Chess/board.hpp"
#include "onlineGame.hpp"

#include <string>
#include <vector>
#include <memory>
#include <random>
#include <iostream>

using std::cout;
using std::make_unique;
using std::string;
using std::vector;
using std::unique_ptr;

void offlineGame(sf::RenderWindow &window, const string &fenStr);

void loadSprites(
   vector< PieceSprite > &sprites,
   const vector< sf::Texture > &textures,
   Chess::Board &board,
   sf::RenderWindow &window
);

char checkPromotion(
   sf::RenderWindow *window,
   vector< PieceSprite > &sprites,
   sf::Sprite &boardSprite,
   unsigned &spritePressedIndex,
   string &to
);

void makeMove(
   Chess::Board &board,
   string &from,
   string &to,
   char pieceToPromoteTo,
   unsigned &piecePressedIndex,
   vector< PieceSprite > &sprites,
   unsigned &spritePressedIndex
);

char getPieceToPromoteTo(
      sf::RenderWindow *window,
      sf::Sprite &boardSprite,
      vector< PieceSprite > *sprites,
      const unsigned &pieceToBePromotedIndex
);

int randomNumber(const int &from, const int &to);

void pickRandomMove(Chess::Board &board, string &from, string &to);

void playRandomMove(Chess::Board &board, string &from, string &to, vector<unique_ptr<PieceSprite>> &sprites);

void playSound(Chess::GameState &gameState, sf::Sound &sound);

#endif
