#ifndef KING_HPP
#define KING_HPP

#include "piece.hpp"

class King : public Piece {
public:
   unsigned shortCastleRookIndex;
   unsigned longCastleRookIndex;
public:
   King(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos);
   void setLegalMoves() override;
   void setRooksIndex();
   void shortCastle() override;
   void longCastle() override;
protected:
   void checkCastle();
};

#endif