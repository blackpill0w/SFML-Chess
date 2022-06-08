#include "queen.hpp"

Queen::Queen(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos)
: Piece(pieces, turn, type, pos)
{
}

void Queen::setLegalMoves() {
   legalMoves.clear();

   /*********************
   *
   *  Loop through every direction (8 in total) and depending on the piece,
   *  the 'isMoveValid()' method checks wether it's a valid move.
   *  If it is, then add it to 'legalMoves' and keep going (until edge of the board),
   *  else, change the direction.
   *  Obviously, this is not for knights.
   *
   *********************/
   char directionX[3] = {0, 1, -1};
   char directionY[3] = {0, 1, -1};

   for (unsigned i=0; i < 3; i++) {
      for (unsigned j=0; j < 3; j++) {
         std::string x{ pos[0] };
         std::string y{ pos[1] };
         std::string temp{ x+y };
         if (i ==0 && j ==0) {
            continue;
         }
         temp[0] = temp[0] + directionX[i];
         temp[1] = temp[1] + directionY[j];

         while (temp[0] >= 'a' && temp[0] <= 'h' && temp[1] >= '1' && temp[1] <= '8') {
            PieceColor pieceOccupyingColor{ isPosOccupied(temp) };
            if ( pieceOccupyingColor != color) {
               legalMoves.push_back(temp);
               // If it's an enemy piece
               if (pieceOccupyingColor != NONE) {
                  break;
               }
            }
            else {
               break;
            }
            temp[0] = temp[0] + directionX[i];
            temp[1] = temp[1] + directionY[j];
         }
      }
   }
}