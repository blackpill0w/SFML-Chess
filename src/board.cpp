#include "board.hpp"

Board::Board() : pieces{}, turn{ WHITE }
{
   pieces.emplace_back(make_unique< Rook >( Rook(&pieces, &turn, 'R', "a1") ));
   pieces.emplace_back(make_unique< Rook >( Rook(&pieces, &turn, 'R', "h1") ));
   pieces.emplace_back(make_unique< Rook >( Rook(&pieces, &turn, 'r', "a8") ));
   pieces.emplace_back(make_unique< Rook >( Rook(&pieces, &turn, 'r', "h8") ));
   pieces.emplace_back(make_unique< King >( King(&pieces, &turn, 'K', "e1") ));
   pieces.emplace_back(make_unique< King >( King(&pieces, &turn, 'k', "e8") ));
   // pieces.emplace_back(make_unique< Queen >( Queen(&pieces, 'Q', "e3") ));
   // pieces.emplace_back(make_unique< Queen >( Queen(&pieces, 'q', "h8") ));
   // pieces.emplace_back(make_unique< Bishop >( Bishop(&pieces, 'B', "a8") ));
   // pieces.emplace_back(make_unique< Bishop >( Bishop(&pieces, 'b', "c6") ));
   // pieces.emplace_back(make_unique< Knight >( Knight(&pieces, 'n', "d6") ));
   // pieces.emplace_back(make_unique< Knight >( Knight(&pieces, &turn, 'N', "b8") ));
   // pieces.emplace_back(make_unique< Pawn >( Pawn(&pieces, &turn, 'P', "a2") ));
   // pieces.emplace_back(make_unique< Pawn >( Pawn(&pieces, &turn, 'P', "b2") ));
   pieces.reserve(3);
   pieces.emplace_back(make_unique< Pawn >( Pawn(&pieces, &turn, 'P', "c7") ));
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

void Board::removePiece(unsigned &index) {
   pieces[index]->alive = false;
   pieces[index]->legalMoves.clear();
   pieces[index]->pos = "  ";
   pieces[index]->attackingPositions[0] = "";
   pieces[index]->attackingPositions[1] = "";
}

bool Board::isMoveCastle(unsigned &movingPieceIndex, string &from, string &to) {
   bool castled{ false };
   if (tolower(pieces[movingPieceIndex]->type) == 'k' && abs(to[0] - from[0]) == 2) {
      castled = true;
      if (to[0] > from[0]) {
         pieces[movingPieceIndex]->shortCastle();
      }
      else {
         pieces[movingPieceIndex]->longCastle();
      }
   }
   return castled;
}

void Board::checkEnableEnPassant(unsigned &movingPieceIndex, string &from, string &to) {
   if (tolower(pieces[movingPieceIndex]->type) == 'p') {
      if (abs(to[1] - from[1]) == 2) {
         pieces[movingPieceIndex]->enPassant = true;
      }
   }
}

void Board::checkDisableEnPassant() {
   for (auto& piece: pieces) {
      // Piece's should equals turn because the turn changes before
      // this function is run
      if (tolower(piece->type) == 'p' && piece->color == turn) {
         if (piece->enPassant) {
            piece->enPassant = false;
         }
      }
   }
}

unsigned Board::checkIfMoveWasEnPassant(unsigned &movingPieceIndex, string &from, string &to) {
   unsigned pieceToBeRemoveIndex{ 65u };

   if (tolower(pieces[movingPieceIndex]->type) == 'p' && to[0] != from[0]) {
      // Making the pos of piece being en-passant-ed (pawn to be taken)
      //** y is 1 step the opposite way the moving (the pawn en-passant-ing) pawn moves
      string y{ pieces[movingPieceIndex]->pos[1] };
      y[0] -= pieces[movingPieceIndex]->movementDirection;

      string EPpos{ pieces[movingPieceIndex]->pos[0] + y };

      for (unsigned i=0; i < pieces.size(); i++) {
         if (tolower(pieces[i]->type) == 'p') {
            if (pieces[i]->enPassant && pieces[i]->pos == EPpos) {
               pieceToBeRemoveIndex = i;
               break;
            }
         }
      }
   }
   return pieceToBeRemoveIndex;
}

void Board::checkTaking(unsigned &movingPieceIndex, string &from, string &to) {
   unsigned pieceToBeRemoveIndex{ 65u };
   pieceToBeRemoveIndex = getIndexOfPieceAt(pieces[movingPieceIndex]->pos, pieces[movingPieceIndex]->color);

   //** Check en passant
   if (pieceToBeRemoveIndex == 65u) {
      pieceToBeRemoveIndex = checkIfMoveWasEnPassant(movingPieceIndex, from, to);
   }
   if (pieceToBeRemoveIndex != 65u) {
      removePiece(pieceToBeRemoveIndex);
   }
}

void Board::checkPromoting(unsigned &movingPieceIndex, const char &pieceToPromoteTo) {
   // What's happening here is basically
   if (pieceToPromoteTo != 0 && tolower(pieces[movingPieceIndex]->type) == 'p') {
      if (pieces[movingPieceIndex]->pos[1] == '1' || pieces[movingPieceIndex]->pos[1] == '8') {
         Piece* newPiece{ nullptr };
         switch (pieceToPromoteTo) {
            case 'Q':
            case 'q':
               newPiece = new Queen(&pieces, &turn, pieceToPromoteTo, pieces[movingPieceIndex]->pos);
               break;
            case 'R':
            case 'r':
               newPiece = new Rook(&pieces, &turn, pieceToPromoteTo, pieces[movingPieceIndex]->pos);
               break;
            case 'B':
            case 'b':
               newPiece = new Bishop(&pieces, &turn, pieceToPromoteTo, pieces[movingPieceIndex]->pos);
               break;
            case 'N':
            case 'n':
               newPiece = new Knight(&pieces, &turn, pieceToPromoteTo, pieces[movingPieceIndex]->pos);
               break;
         }
         pieces[movingPieceIndex].reset( newPiece );
      }
   }
}

bool Board::move(string &from, string &to, const char &pieceToPromoteTo) {
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
      //** If the move is not castle, else, everything is done
      //** inside the isMoveCastle() function
      if ( !isMoveCastle(movingPieceIndex, from, to) ) {
         //** Check wether to enable en passant
         checkEnableEnPassant(movingPieceIndex, from, to);

         // Put the piece in the new position
         pieces[movingPieceIndex]->pos = to;

         //** Take piece is there is any
         checkTaking(movingPieceIndex, from, to);
      }
      // Disable en passant
      checkDisableEnPassant();

      // Check promoting
      checkPromoting(movingPieceIndex, pieceToPromoteTo);
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