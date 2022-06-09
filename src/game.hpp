#ifndef GAME_HPP
#define GAME_HPP

#include "utils.hpp"
#include "board.hpp"

void playGame();

void loadSprites(
   vector< unique_ptr<PieceSprite> > &sprites,
   const vector< sf::Texture > &textures,
   Board &board,
   sf::RenderWindow &window
);

void keepPieceInsideBoard(sf::Vector2f pos);

void draw(
   vector< unique_ptr<PieceSprite> > &sprites,
   unsigned &spritePressedIndex
);

void checkPromotion(
   sf::RenderWindow *window,
   vector< unique_ptr<PieceSprite> > &sprites,
   sf::Sprite &boardSprite,
   unsigned &spritePressedIndex,
   const vector< sf::Texture > &textures,
   char &pieceToPromoteTo,
   string &to
);

void makeMove(
   Board &board,
   string &from,
   string &to,
   char &pieceToPromoteTo,
   unsigned &piecePressedIndex,
   vector< unique_ptr<PieceSprite> > &sprites,
   unsigned &spritePressedIndex
);

char getPieceToPromoteTo(
      sf::RenderWindow *window,
      sf::Sprite &board,
      vector< unique_ptr<PieceSprite> > *sprites,
      const unsigned &pieceToBePromotedIndex
);

#endif