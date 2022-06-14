#include "pawn.hpp"

Pawn::Pawn(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos)
: Piece(pieces, turn, type, pos)
{
}

void Pawn::checkEnPassant() {
   string movingDirection{ pos[1] };
   movingDirection[0] += pawnMovementDirection;

   string right{ pos[0] };
   string left{ pos[0] };
   right[0]++;
   left[0]--;

   string EPpos1{ right + pos[1] };
   string EPpos2{ left + pos[1] };

   for (auto& piece: *pieces) {
      if (piece->color != color && tolower(piece->type) == 'p') {
         if (piece->enPassant) {
            if (piece->pos == EPpos1) {
               // AtattackingPositions are positions where the pawn is attacking a piece,
               // which is the same as en passant
               legalMoves.push_back(attackingPositions[0]);
            }
            else if (piece->pos == EPpos2) {
               EPpos2 = left + movingDirection;
               legalMoves.push_back(attackingPositions[1]);
            }
         }
      }
   }
}

void Pawn::setLegalMoves() {
   //**********************
   //**
   //**  TODO: I'm not sure I like this
   //**
   //**********************
   legalMoves.clear();

   checkEnPassant();

   string x{ pos[0] };
   string y{ pos[1] };
   if (color == WHITE) {
      y[0]++;
   }
   else {
      y[0]--;
   }
   string inFront{ x+y };
   if (!isPosOccupied(inFront)) {
      legalMoves.push_back(inFront);
   }
   if (color == WHITE) {
      y[0]++;
   }
   else {
      y[0]--;
   }
   string twoSteps{ x+y };
   if (!hasMoved && !isPosOccupied(twoSteps)) {
      legalMoves.push_back(twoSteps);
   }
   for (auto& position: attackingPositions){
      PieceColor occupyingPiece{ isPosOccupied(position) };
      if (occupyingPiece != NONE && occupyingPiece != color) {
         legalMoves.push_back(position);
      }
   }
}

void Pawn::updateAttackingPositions() {
   int direction{ 1 };
   if (color == BLACK) {
      direction = -1;
   }
   string x{ pos[0] };
   string y{ pos[1] };

   y[0] += direction;
   x[0]++;
   string right{ x+y };
   // 2 because we already incremented x
   x[0] -= 2;
   string left{ x+y };
   attackingPositions[0] = right;
   attackingPositions[1] = left;
}

void Pawn::update() {
   updateAttackingPositions();
   setLegalMoves();
}