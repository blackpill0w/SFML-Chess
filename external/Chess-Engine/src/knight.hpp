#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "piece.hpp"

namespace Chess
{

class Knight : public Piece {
public:
   Knight(const vector< unique_ptr<Piece> > *pieces, Turn *turn, const char type, const string &pos);
   void setLegalMoves() override;
};

}

#endif
