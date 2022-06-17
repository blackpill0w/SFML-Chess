#ifndef ROOK_HPP
#define ROOK_HPP

#include "piece.hpp"

namespace Chess
{

class Rook : public Piece {
public:
   Rook(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos);
};

}

#endif
