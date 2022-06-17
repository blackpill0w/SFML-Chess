#include "knight.hpp"

namespace Chess
{

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

      string temp{ pos[0] };
      temp += pos[1];
      temp[0] += directions[0];
      temp[1] += directions[1];

      PieceColor occupyingColor{ isPosOccupied(temp) };
      if (occupyingColor != color) {
         legalMoves.push_back(temp);
      }
      else {
         for (auto& piece: *pieces) {
            if (piece->pos == temp && piece->alive) {
               piece->isProtected = true;
            }
         }
      }
   }
}

}
