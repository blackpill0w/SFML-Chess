#include "piece.hpp"

Piece::Piece(const Piece& other)
: type{ other.type }, color{ other.color }, alive{ true }, pos{ other.pos },
  hasMoved{ other.hasMoved }, legalMoves{ "" }, pieces{ other.pieces }, turn{ other.turn },
  enPassant{ other.enPassant }, movementDirection{ other.movementDirection }
{
   // legalMoves
   legalMoves.clear();
   for (auto& move: other.legalMoves) {
      legalMoves.push_back(move);
   }
   // Attacking Pos
   attackingPositions[0] = other.attackingPositions[0];
   attackingPositions[1] = other.attackingPositions[1];
}

Piece::Piece(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos)
: type{ type }, color{ BLACK }, alive{ true }, pos{ pos }, hasMoved{ false },
  legalMoves{ "" }, pieces{ pieces }, turn{ turn }, enPassant{ false }, movementDirection{ 1 }
{
   if (isupper(type)) {
      this->color = WHITE;
   }
   // Useful for pawns
   if (color == BLACK) {
      movementDirection = -1;
   }
}

PieceColor Piece::isPosOccupied(const string &pos) {
   PieceColor colorOfOccupyingPiece{ NONE };
   for (auto& piece: *pieces) {
      if (piece->pos == pos) {
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
      if (piece->color != color) {
         //** Pawns
         //*******************************************************
         //**
         //** When dealing with pawns, we don't check where they can
         //** move like any other piece, because they can't attack
         //** whet's in front of them but rather we check the sides
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
         else if (tolower(piece->type) == 'k') {
            if (
               (abs(position[0] - piece->pos[0]) == 1 || abs(position[0] - piece->pos[0]) == 0)
               && (abs(position[1] - piece->pos[1]) == 1 || abs(position[1] - piece->pos[1]) == 0)
            ) {
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
}

bool Piece::isValidMove(const string &move) {
   return ( find(legalMoves.begin(), legalMoves.end(), move) != legalMoves.end() );
}

//** Only for kings
void Piece::longCastle() {}
void Piece::shortCastle() {}
void Piece::setRooksIndex() {}
