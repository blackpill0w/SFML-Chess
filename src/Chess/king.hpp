#ifndef KING_HPP
#define KING_HPP

#include "piece.hpp"

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
};

#endif
