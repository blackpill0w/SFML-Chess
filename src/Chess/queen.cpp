#include "queen.hpp"

namespace Chess
{

Queen::Queen(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos)
: Piece(pieces, turn, type, pos)
{
}

}
