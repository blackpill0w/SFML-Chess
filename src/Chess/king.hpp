#ifndef KING_HPP
#define KING_HPP

#include "piece.hpp"

namespace Chess
{

class King : public Piece {
public:
   King(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos);
   void setLegalMoves() override;
   void setRooksIndex() override;
   void shortCastle() override;
   void longCastle() override;
protected:
   //********************************************
   //**
   //** Check if castle is possible.
   //**
   //********************************************
   void checkCastle();

   //********************************************
   //**
   //** Check if a piece is protected.
   //**
   //********************************************
   void isPieceProtected();
};

} // Chess namespace

#endif
