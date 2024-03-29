#include "king.hpp"

namespace Chess
{

King::King(const vector< unique_ptr<Piece> > *pieces, Turn *turn, const char type, const string &pos)
	: Piece(pieces, turn, type, pos)
{
	canShortCastle = true;
	canLongCastle = true;
}

void King::setRooksIndex() {
   for (unsigned i=0; i < pieces->size(); i++) {
      if ((*pieces)[i]->color == color && tolower((*pieces)[i]->type) == 'r') {
         if ((*pieces)[i]->pos[0] == 'h') {
            shortCastleRookIndex = i;
         }
         else if ((*pieces)[i]->pos[0] == 'a') {
            longCastleRookIndex = i;
         }
      }
   }
}

void King::checkCastle() {

   if (hasMoved) {
      return;
   }

   // y = 1 for WHITE or y = 8 for BLACK
   string y{ pos[1] };

   if ( shortCastleRookIndex != 65u && !(*pieces)[shortCastleRookIndex]->hasMoved ) {
      // f1/f8 && g1/g8 shouldn't be occupied
      if (isPosOccupied("f"+y) == NONE && isPosOccupied("g"+y) == NONE && canShortCastle) {
         // Also not attacked
         if ( !( canPosBeAttacked("g"+y, color) || canPosBeAttacked("f"+y, color)) ) {
            legalMoves.emplace_back("g"+y);
         }
      }
   }
   if ( shortCastleRookIndex != 65u && !(*pieces)[longCastleRookIndex]->hasMoved && canLongCastle ) {
      // b1/b8 && c1/c8 && d1/d8 shouldn't be occupied
      if (isPosOccupied("b"+y) == NONE && isPosOccupied("c"+y) == NONE && isPosOccupied("d"+y) == NONE) {
         // c1/c8 && d1/d8 souldn't be attacked
         if ( !( canPosBeAttacked("c"+y, color) || canPosBeAttacked("d"+y, color)) ) {
            legalMoves.emplace_back("c"+y);
         }
      }
   }
}

void King::shortCastle() {
   pos[0] = 'g';
   (*pieces)[shortCastleRookIndex]->pos[0] = 'f';
   (*pieces)[shortCastleRookIndex]->hasMoved = true;
}

void King::longCastle() {
   pos[0] = 'c';
   (*pieces)[longCastleRookIndex]->pos[0] = 'd';
   (*pieces)[longCastleRookIndex]->hasMoved = true;
}

void King::setLegalMoves() {
   legalMoves.clear();

   char directionX[3] = {0, 1, -1};
   char directionY[3] = {0, 1, -1};

   for (unsigned i=0; i < 3; i++) {
      for (unsigned j=0; j < 3; j++) {
         string temp{ pos[0] };
         temp += pos[1];

         if (i == 0 && j == 0) {
            continue;
         }
         temp[0] += directionX[i];
         temp[1] += directionY[j];
         if (temp[0] != 'h'+1 && temp[0] != 'a'-1 && temp[1] != '0' && temp[1] != '9') {
            PieceColor pieceOccupyingColor{ isPosOccupied(temp) };
            bool canKingBeAttacked{ canPosBeAttacked(temp, color) };

            if ( pieceOccupyingColor != color && !canKingBeAttacked) {
               // If it's an enemy piece, we should check if it's protected
               if (pieceOccupyingColor != NONE) {
                  for (auto& piece: *pieces) {
                     if (piece->pos == temp && piece->alive) {
                        if (!piece->isProtected) {
                           legalMoves.emplace_back(temp);
                        }
                     }
                  }
               }
               else {
                  legalMoves.emplace_back(temp);
               }
            }
         }
      }
   }
   checkCastle();
}

} // Chess namespace
