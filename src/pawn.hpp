#ifndef PAWN_HPP
#define PAWN_HPP

#include "piece.hpp"

class Pawn : public Piece {
public:
   Pawn(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos);
   void setLegalMoves() override;
   void update() override;
protected:
   void updateAttackingPositions();
   void checkEnPassant();
};

#endif