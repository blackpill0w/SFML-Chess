#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>

#include "pieces.hpp"

using std::cout;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;
using std::find;

class Board {

public:
   vector< unique_ptr<Piece> > pieces;
   PieceColor turn;
public:
   Board();
   void changeTurn();
   bool move(std::string &from, std::string &to);
   unsigned getIndexOfPieceAt(const string &pos, const PieceColor &colorToBeDifferentFrom = NONE);
   void update();
protected:
   void removePiece(unsigned index);
   void castle(unsigned movingPieceIndex, string from, string to);
};

#endif