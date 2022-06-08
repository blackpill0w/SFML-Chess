#include "knight.hpp"

Knight::Knight(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos)
: Piece(pieces, turn, type, pos)
{
}

void Knight::setLegalMoves() {
   legalMoves.clear();

   char movesDirections[8][2] = {
      {1, 2}, {-1, 2},
      {1, -2}, {-1, -2},
      {2, 1}, {-2, 1},
      {2, -1}, {-2, -1}
   };
   for (auto& directions: movesDirections) {
      string x{ pos[0] };
      x[0] += directions[0];
      string y{ pos[1] };
      y[0] += directions[1];
      string temp{ x+y };
      PieceColor occupyingColor{ isPosOccupied(temp) };
      if (occupyingColor != color) {
         legalMoves.push_back(temp);
      }
   }
}