#include "piece.hpp"

#include <algorithm>
using std::find;

namespace Chess
{

Piece::Piece(const vector< unique_ptr<Piece> > *pieces, Turn *turn, const char type, const string &pos)
: type{ type }, color{ BLACK }, alive{ true }, pos{ pos }, hasMoved{ false },
  legalMoves{ "" }, pieces{ pieces },
  turn{ turn }, isProtected{ false }, initialPos{ pos },
  enPassant{ false }, pawnMovementDirection{ 1 }, inCheck{ false },
  canShortCastle{ false }, canLongCastle{ false }
{
   if (isupper(type)) {
      this->color = WHITE;
   }
   // Useful for pawns
   if (color == BLACK) {
      pawnMovementDirection = -1;
   }
}

PieceColor Piece::isPosOccupied(const string &pos) {
   PieceColor colorOfOccupyingPiece{ NONE };
   for (auto& piece: *pieces) {
      if (piece->pos == pos && piece->alive) {
         colorOfOccupyingPiece = piece->color;
         break;
      }
   }
   return colorOfOccupyingPiece;
}

bool Piece::canPosBeAttacked(const string &position, const PieceColor &color) {
   bool canBeAttacked{ false };
   for (auto& piece: *pieces) {
      if (canBeAttacked) {
         break;
      }
      if (piece->color != color && piece->alive) {
         //** Pawns
         //*******************************************************
         //**
         //** When dealing with pawns, we don't check where they can
         //** move like any other piece, because they can't attack
         //** what's in front of them but rather we check the sides
         //**
         //*******************************************************
         if (tolower(piece->type) == 'p') {
            if (position == piece->attackingPositions[0] || position == piece->attackingPositions[1]) {
               canBeAttacked = true;
            }
         }
         //** King
         //*******************************************************
         //**
         //** Kings might be unable to move to some positions
         //** usually because they can be by other pieces (that are not kings),
         //** which would allow the other king to be able to move there
         //** which is illegal, kings should NOT touch each other -_o
         //**
         //*******************************************************
         else if (tolower(piece->type) == 'k' && piece->alive) {
            if (abs(position[0] - piece->pos[0]) <= 1 && (abs(position[1] - piece->pos[1]) <= 1)) {
               canBeAttacked = true;
            }
         }
         //** Other pieces
         //*******************************************************
         //**
         //** Queens, rooks, bishops and knights.
         //**
         //*******************************************************
         else {
            if (find(piece->legalMoves.begin(), piece->legalMoves.end(), position) != piece->legalMoves.end()) {
               canBeAttacked = true;
            }
         }
      }
   }
   return canBeAttacked;
}

void Piece::update() {
   setLegalMoves();
   removeMovesOutsideBoard();
   if (inCheck) {
      inCheck = false;
   }
}

bool Piece::isValidMove(const string &move) {
   return ( find(legalMoves.begin(), legalMoves.end(), move) != legalMoves.end() );
}

void Piece::removeMovesOutsideBoard() {
   for (unsigned i=0; i != legalMoves.size();) {
      if (legalMoves[i][0] > 'h' || legalMoves[i][0] < 'a' || legalMoves[i][1] > '8' || legalMoves[i][1] < '1') {
         legalMoves.erase(legalMoves.begin() + i);
      }
      else {
         i++;
      }
   }
}

void Piece::setLegalMoves() {
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
         // Rooks
         if (tolower(type) == 'r' && directionX[i] != 0 && directionY[j] != 0) {
            continue;
         }
         // Bishops
         if (tolower(type) == 'b' && (directionX[i] == 0 || directionY[j] == 0)) {
            continue;
         }
         if (i ==0 && j ==0) {
            continue;
         }
         string temp{ pos[0] };
         temp += pos[1];
         temp[0] += directionX[i];
         temp[1] += directionY[j];

         while (temp[0] >= 'a' && temp[0] <= 'h' && temp[1] >= '1' && temp[1] <= '8') {
            PieceColor pieceOccupyingColor{ isPosOccupied(temp) };
            if ( pieceOccupyingColor != color) {
               legalMoves.emplace_back(temp);
               // If it's an enemy piece
               if (pieceOccupyingColor != NONE) {
                  break;
               }
            }
            else {
               // Mark the piece protected
               for (auto& piece: *pieces) {
                  if (piece->pos == temp && piece->alive) {
                     piece->isProtected = true;
                     break;
                  }
               }
               break;
            }
            temp[0] = temp[0] + directionX[i];
            temp[1] = temp[1] + directionY[j];
         }
      }
   }
}

void Piece::reset() {
   pos = initialPos;
   alive = true;
}

//** Only for kings
void Piece::longCastle() {}
void Piece::shortCastle() {}
void Piece::setRooksIndex() {}

} // Chess namespace
