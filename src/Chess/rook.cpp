#include "rook.hpp"

namespace Chess
{

Rook::Rook(const vector< unique_ptr<Piece> > *pieces, Turn *turn, const char type, const string &pos)
: Piece(pieces, turn, type, pos)
{
}

}
