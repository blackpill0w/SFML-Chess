#include "pawn.hpp"

namespace Chess
{

Pawn::Pawn(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos)
: Piece(pieces, turn, type, pos)
{
}

bool Pawn::isEnPassantResultsInCheck() {
   //********************************************
   //**
   //** Sometimes en passant results in the king
   //** being attacked.
   //** ex: Q - - P p - k -
   //** If en passant happens, both pawns move
   //** and the queen is able to take the king.
   //**
   //********************************************

   constexpr int directions[2] = {1, -1};
   bool kingToTheSide{ false };
   bool attackingPieceToTheSide{ false };
   for (auto& direction: directions) {
      string temp{ pos };
      temp[0] += direction;
      while (temp[0] != ('h'+1) && temp[0] != ('a'-1)) {
         for (auto& piece: *pieces) {
            if (piece->pos == temp) {
               if (piece->color == color && tolower(piece->type) == 'k') {
                  kingToTheSide = true;
                  break;
               }
               else if (piece->color != color && (tolower(piece->type) == 'q' || tolower(piece->type) == 'r') && piece->alive) {
                  attackingPieceToTheSide = true;
                  break;
               }
            }
         }
         temp[0] += direction;
      }
   }
   if (kingToTheSide && attackingPieceToTheSide) {
      return true;
   }
   return false;
}

void Pawn::checkEnPassant() {
   void undoLastMove();
   string EPpos1{ pos };
   string EPpos2{ pos };
   // Right
   EPpos1[0] += 1;
   // Left
   EPpos2[0] += (-1);

   for (auto& piece: *pieces) {
      if (piece->color != color && tolower(piece->type) == 'p') {
         if (piece->enPassant) {
            if (piece->pos == EPpos1) {
               // AtattackingPositions are positions where the pawn is attacking a piece,
               // which is the same as en passant
               if (!isEnPassantResultsInCheck()) {
                  legalMoves.push_back(attackingPositions[0]);
               }
            }
            else if (piece->pos == EPpos2) {
               if (!isEnPassantResultsInCheck()) {
                  legalMoves.push_back(attackingPositions[1]);
               }
            }
         }
      }
   }
}

void Pawn::setLegalMoves() {
   legalMoves.clear();

   checkEnPassant();

   string inFront{ pos[0] };
   inFront += (pos[1] + pawnMovementDirection);
   if (!isPosOccupied(inFront)) {
      legalMoves.push_back(inFront);
      // Two squares forward
      if (!hasMoved) {
         string twoSteps{ inFront };
         twoSteps[1] += pawnMovementDirection;
         if (isPosOccupied(twoSteps) == NONE ) {
            legalMoves.push_back(twoSteps);
         }
      }
   }
   for (auto& position: attackingPositions){
      PieceColor occupyingPiece{ isPosOccupied(position) };
      if (occupyingPiece != NONE && occupyingPiece != color) {
         legalMoves.push_back(position);
      }
      else {
         for (auto& piece: *pieces) {
            if (piece->pos == position && piece->alive) {
               piece->isProtected = true;
            }
         }
      }
   }
}

void Pawn::updateAttackingPositions() {
   string right{ pos };
   right[0] += 1;
   right[1] += pawnMovementDirection;
   string left{ pos };
   left[0] += (-1);
   left[1] += pawnMovementDirection;
   attackingPositions[0] = right;
   attackingPositions[1] = left;
}

void Pawn::update() {
   updateAttackingPositions();
   setLegalMoves();
}

} // Chess namespace
