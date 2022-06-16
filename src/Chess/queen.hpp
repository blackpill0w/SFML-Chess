#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "piece.hpp"

class Queen : public Piece {
public:
   Queen(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos);
};

#endif
