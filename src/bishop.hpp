#ifndef BISHOP_HPP
#define BISHOP_HPP

#include "piece.hpp"

class Bishop : public Piece {
public:
   Bishop(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos);
   void setLegalMoves() override;
};

#endif