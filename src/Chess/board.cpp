#include "board.hpp"

Board::Board( const string &fenStr ) :
   pieces{}, turn{ WHITE },
   wKingIndex{ 0u }, bKingIndex{ 0u }, moveList{}, possibleMoves{ 0u }
{
   if (fenStr != "") {
      loadFEN(fenStr);
      pieces[wKingIndex]->setRooksIndex();
      pieces[bKingIndex]->setRooksIndex();
   }
   // Calculate pieces' moves
   this->update();
}

Board::Board(const Board &other)
: pieces{}, turn{ other.turn }, wKingIndex{ other.wKingIndex },
  bKingIndex{ other.bKingIndex }, moveList{ other.moveList }, possibleMoves{ other.possibleMoves }
{
   // Copy the pieces
   for (unsigned i=0; i < other.pieces.size(); i++) {
      if (tolower(other.pieces[i]->type) == 'k') {
         pieces.emplace_back( make_unique<King>( King(&pieces, &turn, other.pieces[i]->type, other.pieces[i]->pos) ) );
      }
      else if (tolower(other.pieces[i]->type) == 'q') {
         pieces.emplace_back( make_unique<Queen>( Queen(&pieces, &turn, other.pieces[i]->type, other.pieces[i]->pos) ) );
      }
      else if (tolower(other.pieces[i]->type) == 'r') {
         pieces.emplace_back( make_unique<Rook>( Rook(&pieces, &turn, other.pieces[i]->type, other.pieces[i]->pos) ) );
      }
      else if (tolower(other.pieces[i]->type) == 'b') {
         pieces.emplace_back( make_unique<Bishop>( Bishop(&pieces, &turn, other.pieces[i]->type, other.pieces[i]->pos) ) );
      }
      else if (tolower(other.pieces[i]->type) == 'n') {
         pieces.emplace_back( make_unique<Knight>( Knight(&pieces, &turn, other.pieces[i]->type, other.pieces[i]->pos) ) );
      }
      else if (tolower(other.pieces[i]->type) == 'p') {
         pieces.emplace_back( make_unique<Pawn>( Pawn(&pieces, &turn, other.pieces[i]->type, other.pieces[i]->pos) ) );
      }
   }
}

void Board::loadFEN(const string &fenStr) {
   enum { wKingSide = 0, wQueenSide, bKingSide, bQueenSide };

   string x{ "a" };
   string y{ "8" };

   for (auto& c: fenStr) {
      if (c == '/') {
         x = "a";
         y[0] -= 1;
      }
      else if (isdigit(c)) {
         x[0] += ( (c - '1') + 1 );
      }
      // Placing pieces
      else {
         string pos{ x+y };
         x[0] += 1;
         switch (tolower(c)) {
            case 'k':
               pieces.emplace_back( make_unique< King >( King(&pieces, &turn, c, pos) ) );
               // Save kings' indices
               if (isupper(c)) {
                  wKingIndex = pieces.size() - 1;
               }
               else {
                  bKingIndex = pieces.size() - 1;
               }
               break;
            case 'q':
               pieces.emplace_back( make_unique< Queen >( Queen(&pieces, &turn, c, pos) ) );
               break;
            case 'r':
               pieces.emplace_back( make_unique< Rook >( Rook(&pieces, &turn, c, pos) ) );
               break;
            case 'b':
               pieces.emplace_back( make_unique< Bishop >( Bishop(&pieces, &turn, c, pos) ) );
               break;
            case 'n':
               pieces.emplace_back( make_unique< Knight >( Knight(&pieces, &turn, c, pos) ) );
               break;
            case 'p':
               pieces.emplace_back( make_unique< Pawn >( Pawn(&pieces, &turn, c, pos) ) );
               break;
            default:
               // Do nothing, just return
               cout << "Invalid FEN character: "<< c << '\n';
               cout << "quiting...\n";
               return;
               break;
         }
      }
   }
}

unsigned Board::getIndexOfPieceAt(const string &pos, const PieceColor &colorToBeDifferentFrom, const bool &aliveState) {
   unsigned pieceIndex{ 65u };
   for (unsigned i=0; i < pieces.size(); i++) {
      if (pieces[i]->alive == aliveState) {
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

void Board::removePiece(const unsigned &index) {
   pieces[index]->alive = false;
}

bool Board::isMoveCastle(const unsigned &movingPieceIndex, const string &from, const string &to) {
   //** King can only move to squares next to it
   //** meaning, if the difference between the old position and the new is > 2
   //** then, it was castle
   return (tolower(pieces[movingPieceIndex]->type) == 'k' && abs(to[0] - from[0]) == 2);
}

void Board::handleCastling(const unsigned &movingPieceIndex, const string &from, const string &to) {
   if (to[0] > from[0]) {
      pieces[movingPieceIndex]->shortCastle();
   }
   else {
      pieces[movingPieceIndex]->longCastle();
   }
}

void Board::checkEnableEnPassant(const unsigned &movingPieceIndex, const string &from, const string &to) {
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
      if (tolower(piece->type) == 'p' && piece->color == turn && piece->alive) {
         if (piece->enPassant) {
            piece->enPassant = false;
         }
      }
   }
}

unsigned Board::checkIfMoveWasEnPassant(const unsigned &movingPieceIndex, const string &from, const string &to) {
   unsigned pieceToBeRemoveIndex{ 65u };

   if (tolower(pieces[movingPieceIndex]->type) == 'p' && to[0] != from[0]) {
      // Making the pos of piece being en-passant-ed (pawn to be taken)
      //** y is 1 step the opposite way the moving (the pawn en-passant-ing) pawn moves
      string y{ pieces[movingPieceIndex]->pos[1] };
      y[0] -= pieces[movingPieceIndex]->pawnMovementDirection;

      string EPpos{ pieces[movingPieceIndex]->pos[0] + y };

      for (unsigned i=0; i < pieces.size(); i++) {
         if (tolower(pieces[i]->type) == 'p' && pieces[i]->alive) {
            if (pieces[i]->enPassant && pieces[i]->pos == EPpos) {
               pieceToBeRemoveIndex = i;
               break;
            }
         }
      }
   }
   return pieceToBeRemoveIndex;
}

void Board::checkTaking(const unsigned &movingPieceIndex, const string &from, const string &to, MoveData &move) {
   unsigned pieceToBeRemoveIndex{ 65u };
   bool isEnPassant{ false };
   pieceToBeRemoveIndex = getIndexOfPieceAt(pieces[movingPieceIndex]->pos, pieces[movingPieceIndex]->color);

   //** Check en passant
   if (pieceToBeRemoveIndex == 65u) {
      pieceToBeRemoveIndex = checkIfMoveWasEnPassant(movingPieceIndex, from, to);
      isEnPassant = true;
   }
   if (pieceToBeRemoveIndex != 65u) {
      removePiece(pieceToBeRemoveIndex);
      move.takenPieceIndex = pieceToBeRemoveIndex;
      if (isEnPassant) {
         move.isMoveEnPassant = true;
      }
   }
}

void Board::checkPromoting(const unsigned &movingPieceIndex, const char &pieceToPromoteTo, MoveData &move) {
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
         move.isPromotion = true;
      }
   }
}

bool Board::move(const string &from, const string &to, const char &pieceToPromoteTo) {
   bool moveSuccess{ false };
   unsigned movingPieceIndex{ 65u }; // More than a chess board can contain

   movingPieceIndex = getIndexOfPieceAt(from);
   if (movingPieceIndex == 65u) {
      return false;
   }
   if( pieces[movingPieceIndex]->color == turn && pieces[movingPieceIndex]->isValidMove(to) ) {
      // To save the move
      MoveData move;
      move.movingPieceIndex = movingPieceIndex;
      move.oldPos = from;
      move.newPos = to;

      // Moved successfully
      moveSuccess = true;
      if (!pieces[movingPieceIndex]->hasMoved) {
         pieces[movingPieceIndex]->hasMoved = true;
         move.pieceHasMovedChanged = true;
      }
      changeTurn();
      //** If the move is not castle, else, everything is done
      //** inside the isMoveCastle() function
      if ( !isMoveCastle(movingPieceIndex, from, to) ) {
         //** Check wether to enable en passant
         checkEnableEnPassant(movingPieceIndex, from, to);

         // Put the piece in the new position
         pieces[movingPieceIndex]->pos = to;

         //** Take piece is there is any
         checkTaking(movingPieceIndex, from, to, move);
      }
      else {
         handleCastling(movingPieceIndex, from, to);
         move.isMoveCastle = true;
      }
      // Disable en passant
      checkDisableEnPassant();

      // Check promoting
      checkPromoting(movingPieceIndex, pieceToPromoteTo, move);

      moveList.push_back(move);
   }
   if (moveSuccess) {
      // Recalculate pieces' moves
      update();
   }
   return moveSuccess;
}

void Board::update() {
   for (auto& piece: pieces) {
      if (piece->alive){
         piece->update();
      }
   }
   pieces[wKingIndex]->update();
   pieces[bKingIndex]->update();
   handlePins();
   handleChecks();
   calculatePossibleMoves();
}

void Board::undoLastMove() {
   if (!moveList.empty()) {
      MoveData lastMove = { moveList[moveList.size() - 1u] };
      // Type of pawn if move was a promotion
      char type{ 'P' };
      if (lastMove.takenPieceIndex != 65u) {
         pieces[lastMove.takenPieceIndex]->alive = true;
      }
      if (islower(pieces[lastMove.movingPieceIndex]->type)) {
         type = 'p';
      }

      if (lastMove.isPromotion) {
         pieces[lastMove.movingPieceIndex].reset(new Pawn(&pieces, &turn, type, lastMove.oldPos));
      }
      // If move was not a promotion
      else {
         pieces[lastMove.movingPieceIndex]->pos = lastMove.oldPos;
         // Check if the class member 'hasMoved' changed
         if (lastMove.pieceHasMovedChanged) {
            pieces[lastMove.movingPieceIndex]->hasMoved = false;
         }
         // Handle castling
         if (lastMove.isMoveCastle) {
            unsigned rookIndex{ 0u };
            string oldRookPos{ "XY" };
            oldRookPos[1] = lastMove.oldPos[1];
            // Short castle
            if (lastMove.newPos[0] == 'g') {
               rookIndex = pieces[lastMove.movingPieceIndex]->shortCastleRookIndex;
               oldRookPos[0] = 'h';
            }
            // Long castle
            else {
               rookIndex = pieces[lastMove.movingPieceIndex]->longCastleRookIndex;
               oldRookPos[0] = 'a';
            }
            pieces[rookIndex]->hasMoved = false;
            pieces[rookIndex]->pos = oldRookPos;
         }
         if (lastMove.isMoveEnPassant) {
            // We already brought it back to the board
            pieces[lastMove.takenPieceIndex]->enPassant = true;
         }
      }

      moveList.pop_back();
      changeTurn();
   }
   // Recalculate possible moves
   update();
}

void Board::removeMovesIfNotInVector(const unsigned &pieceIndex, const vector< string > &moves) {
   for (unsigned i=0; i < pieces[pieceIndex]->legalMoves.size();) {
      if (find(moves.begin(), moves.end(), pieces[pieceIndex]->legalMoves[i] ) != moves.end()) {
         i++;
      }
      else {
         // No need to increment 'i' because all elements are shifted after deletion of move
         pieces[pieceIndex]->legalMoves.erase( pieces[pieceIndex]->legalMoves.begin() + i );
      }
   }
}

vector< string > Board::getAllSquaresInBetween(string &from, string &to, const int direction[2]) {
   vector< string > moves{};
   string temp{ from[0] };
   temp += from[1];
   while (temp != to) {
      temp[0] += direction[0];
      temp[1] += direction[1];

      moves.emplace_back(temp);
   }
   return moves;
}

void Board::handlePins() {
   //********************************************
   //**
   //** Loop through the eight directions, if we find
   //** a friend piece, we keep looking until we find another piece
   //** (or the edge of the board), if the piece is an enemy
   //** piece and can attack (depending on the direction),
   //** remove all the moves for the friend piece except
   //** the ones between the king and the attacking piece.
   //**
   //********************************************
   constexpr int directions[8][2] = {
      {1, 0}, {-1, 0},
      {0, 1}, {0, -1},
      {1, 1}, {1, -1},
      {-1, 1}, {-1, -1}
   };
   unsigned kingIndex{ turn == WHITE ? wKingIndex : bKingIndex };

   for (unsigned i=0u; i < 8u; i++) {
      string temp{ pieces[kingIndex]->pos };
      temp[0] += directions[i][0];
      temp[1] += directions[i][1];

      bool checkingForPinnedPiece{ true };
      unsigned pinnedPieceIndex{ 65u };

      while (temp[0] != ('h'+1) && temp[0] != ('a'-1) && temp[1] != '0' && temp[1] != '9') {
         unsigned pieceIndex = getIndexOfPieceAt(temp);

         if (pieceIndex != 65u) {
            if (checkingForPinnedPiece) { // Looking for a piece to pin
               if (pieces[pieceIndex]->color == pieces[kingIndex]->color) {
                  pinnedPieceIndex = pieceIndex;
                  checkingForPinnedPiece = false;
               }
               else {
                  break;
               }
            }
            else { // Looking for the pinning piece
               if (pieces[pieceIndex]->color != pieces[kingIndex]->color) {
                  // The first 4 directions are horizontal and vertical
                  // The other 4 are diagonals
                  if (
                     tolower(pieces[pieceIndex]->type) == 'q'
                     || (i < 4 && tolower(pieces[pieceIndex]->type) == 'r')
                     || (i >= 4 && tolower(pieces[pieceIndex]->type) == 'b')
                  ) {
                     vector<string> legalSquares{
                        getAllSquaresInBetween(pieces[kingIndex]->pos, pieces[pieceIndex]->pos, directions[i])
                     };
                     removeMovesIfNotInVector(pinnedPieceIndex, legalSquares);
                  }
               }
               break;
            }
         }
         temp[0] += directions[i][0];
         temp[1] += directions[i][1];
      }
   }


}

void Board::handleChecks() {
   handleSlidingPiecesChecks();
   handlePawnKnightCheck();
}

void Board::handleSlidingPiecesChecks() {
   //********************************************
   //**
   //** Loop through the eight directions, if we find
   //** an enemy piece attacking the king,
   //** remove all moves of the friend pieces
   //** except the ones between the king and the attacking piece.
   //**
   //********************************************
   constexpr int directions[8][2] = {
      {1, 0}, {-1, 0},
      {0, 1}, {0, -1},
      {1, 1}, {1, -1},
      {-1, 1}, {-1, -1}
   };
   unsigned kingIndex{ turn == WHITE ? wKingIndex : bKingIndex };
   for (unsigned i=0u; i < 8u; i++) {
      string temp{ pieces[kingIndex]->pos };
      temp[0] += directions[i][0];
      temp[1] += directions[i][1];

      while (temp[0] != ('h'+1) && temp[0] != ('a'-1) && temp[1] != '0' && temp[1] != '9') {
         unsigned pieceIndex = getIndexOfPieceAt(temp);
         if (pieceIndex != 65u) {
            if (pieces[pieceIndex]->color != pieces[kingIndex]->color) {
               // The first 4 directions are horizontal and vertical
               // The other 4 are diagonals
               if (
                  tolower(pieces[pieceIndex]->type) == 'q'
                  || (i < 4 && tolower(pieces[pieceIndex]->type) == 'r')
                  || (i >= 4 && tolower(pieces[pieceIndex]->type) == 'b')
               ) {
                  vector<string> legalSquares{
                     getAllSquaresInBetween(pieces[kingIndex]->pos, pieces[pieceIndex]->pos, directions[i])
                  };
                  for (unsigned j=0u; j < pieces.size(); j++) {
                     if (pieces[j]->color == turn && pieces[j]->alive && pieces[j]->type != pieces[kingIndex]->type) {
                        removeMovesIfNotInVector(j, legalSquares);
                     }
                  }

                  //*****
                  //** King might still be able to move to a position that is attacked
                  //** ex: if we have a queen at <a1> and a king at <g1> (attacked),
                  //**     the king is still able to go to <h1>
                  //**     Q - - - - - - k ~
                  //*****
                  string temp2{ pieces[kingIndex]->pos };
                  temp2[0] -= directions[i][0];
                  temp2[1] -= directions[i][1];
                  for (unsigned i=0u; i < pieces[kingIndex]->legalMoves.size(); i++) {
                     if (pieces[kingIndex]->legalMoves[i] == temp2) {
                        pieces[kingIndex]->legalMoves.erase(pieces[kingIndex]->legalMoves.begin() + i);
                        break;
                     }
                  }

               }
            }
            break;
         }

         temp[0] += directions[i][0];
         temp[1] += directions[i][1];
      }
   }
}

void Board::handlePawnKnightCheck() {
   vector< string > attackingPiecesPos{};
   unsigned kingIndex{ turn == WHITE ? wKingIndex : bKingIndex };
   for (auto& attackingPiece: pieces) {
      if (attackingPiece->color != pieces[kingIndex]->color && attackingPiece->alive) {
         if ( tolower(attackingPiece->type) == 'p' || tolower(attackingPiece->type) == 'n' ) {
            if (find(attackingPiece->legalMoves.begin(), attackingPiece->legalMoves.end(), pieces[kingIndex]->pos) != attackingPiece->legalMoves.end()) {
               attackingPiecesPos.emplace_back(attackingPiece->pos);
            }
         }
      }
   }
   if (! attackingPiecesPos.empty()) {
      for (unsigned i=0u; i < pieces.size(); i++) {
         if (pieces[i]->color == pieces[kingIndex]->color && pieces[i]->pos != pieces[kingIndex]->pos) {
            removeMovesIfNotInVector(i, attackingPiecesPos);
         }
      }
   }
}

void Board::calculatePossibleMoves() {
   possibleMoves = 0u;
   for (auto& piece: pieces) {
      if (piece->color == turn && piece->alive) {
         possibleMoves += piece->legalMoves.size();
      }
   }
}

unsigned Board::getNumberOfPossibleMoves() {
   return possibleMoves;
}
