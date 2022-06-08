#include "board.hpp"

Board::Board() : pieces{}, turn{ WHITE }
{
   // pieces.emplace_back(make_unique< Rook >( Rook(&pieces, 'R', "a1") ));
   // pieces.emplace_back(make_unique< Rook >( Rook(&pieces, 'R', "h1") ));
   // pieces.emplace_back(make_unique< Rook >( Rook(&pieces, 'r', "a8") ));
   // pieces.emplace_back(make_unique< Rook >( Rook(&pieces, 'r', "h8") ));
   // pieces.emplace_back(make_unique< King >( King(&pieces, 'K', "e1") ));
   // pieces.emplace_back(make_unique< King >( King(&pieces, 'k', "e8") ));
   // pieces.emplace_back(make_unique< Queen >( Queen(&pieces, 'Q', "e3") ));
   // pieces.emplace_back(make_unique< Queen >( Queen(&pieces, 'q', "h8") ));
   // pieces.emplace_back(make_unique< Bishop >( Bishop(&pieces, 'B', "a8") ));
   // pieces.emplace_back(make_unique< Bishop >( Bishop(&pieces, 'b', "c6") ));
   // pieces.emplace_back(make_unique< Knight >( Knight(&pieces, 'n', "d6") ));
   // pieces.emplace_back(make_unique< Knight >( Knight(&pieces, &turn, 'N', "b8") ));
   // pieces.emplace_back(make_unique< Pawn >( Pawn(&pieces, &turn, 'P', "a2") ));
   // pieces.emplace_back(make_unique< Pawn >( Pawn(&pieces, &turn, 'P', "b2") ));
   pieces.reserve(3);
   pieces.emplace_back(make_unique< Pawn >( Pawn(&pieces, &turn, 'P', "c2") ));
   pieces.emplace_back(make_unique< Pawn >( Pawn(&pieces, &turn, 'p', "a6") ));
   pieces.emplace_back(make_unique< Pawn >( Pawn(&pieces, &turn, 'p', "b6") ));
   // pieces.emplace_back(make_unique< Pawn >( Pawn(&pieces, &turn, 'p', "c6") ));
}

unsigned Board::getIndexOfPieceAt(const string &pos, const PieceColor &colorToBeDifferentFrom) {
   unsigned pieceIndex{ 65u };
   for (unsigned i=0; i < pieces.size(); i++) {
      if (pieces[i]->alive) {
         if (pieces[i]->pos == pos && pieces[i]->color != colorToBeDifferentFrom) {
            pieceIndex = i;
            break;
         }
      }
   }
   return pieceIndex;
}

void Board::changeTurn() {
   if (turn == WHITE) {
      turn = BLACK;
   }
   else {
      turn = WHITE;
   }
}

void Board::removePiece(unsigned index) {
   pieces[index]->alive = false;
   pieces[index]->legalMoves.clear();
   pieces[index]->pos = "  ";
   pieces[index]->attackingPositions[0] = "";
   pieces[index]->attackingPositions[1] = "";
}

void Board::castle(unsigned movingPieceIndex, string from, string to) {
   if (to[0] > from[0]) {
      pieces[movingPieceIndex]->shortCastle();
   }
   else {
      pieces[movingPieceIndex]->longCastle();
   }
}

bool Board::move(std::string &from, std::string &to) {
   bool moveSuccess{ false };
   unsigned movingPieceIndex{ 65u }; // More than a chess board can contain

   movingPieceIndex = getIndexOfPieceAt(from);
   if (movingPieceIndex == 65u) {
      return false;
   }
   if( pieces[movingPieceIndex]->isValidMove(to) ) {
      // Moved successfully
      moveSuccess = true;
      pieces[movingPieceIndex]->hasMoved = true;
      changeTurn();
      // Check castling
      //** If it's a king and moving away 2 steps
      if (tolower(pieces[movingPieceIndex]->type) == 'k' && abs(to[0] - from[0]) == 2) {
         castle(movingPieceIndex, from, to);
      }
      else {
         //** Check wether to enable en passant
         if (tolower(pieces[movingPieceIndex]->type) == 'p') {
            if (abs(to[1] - from[1]) == 2) {
               pieces[movingPieceIndex]->enPassant = true;
            }
         }
         pieces[movingPieceIndex]->pos = to;
         //** Take piece is there is any
         unsigned pieceToRemoveIndex{ 65u };
         pieceToRemoveIndex = getIndexOfPieceAt(pieces[movingPieceIndex]->pos, pieces[movingPieceIndex]->color);
         //** Check en passant
         if (pieceToRemoveIndex == 65u && tolower(pieces[movingPieceIndex]->type) == 'p' && to[0] != from[0]) {
            // Making the pos of piece being en-passant-ed (pawn to be taken)
            //** y is 1 step the opposite way the moving (the pawn en-passant-ing) pawn moves
            string y{ pieces[movingPieceIndex]->pos[1] };
            y[0] -= pieces[movingPieceIndex]->movementDirection;

            string EPpos{ pieces[movingPieceIndex]->pos[0] + y };

            for (unsigned i=0; i < pieces.size(); i++) {
               if (tolower(pieces[i]->type) == 'p') {
                  if (pieces[i]->enPassant && pieces[i]->pos == EPpos) {
                     pieceToRemoveIndex = i;
                     break;
                  }
               }
            }
         }
         if (pieceToRemoveIndex < pieces.size()) {
            removePiece(pieceToRemoveIndex);
         }
      }
      // Disable en passant
      for (auto& piece: pieces) {
         if (tolower(piece->type) == 'p' && piece->color != pieces[movingPieceIndex]->color) {
            if (piece->enPassant) {
               piece->enPassant = false;
            }
         }
      }
   }
   return moveSuccess;
}

void Board::update() {
   for (auto& piece: pieces) {
      if (piece->alive){
         piece->update();
      }
   }
}