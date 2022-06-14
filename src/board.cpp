#include "board.hpp"

Board::Board( const string &fenStr ) : pieces{}, turn{ WHITE }, wKingIndex{ 0u }, bKingIndex{ 0u }, moveList{}
{
   if (fenStr != "") {
      loadFEN(fenStr);
      pieces[wKingIndex]->setRooksIndex();
      pieces[bKingIndex]->setRooksIndex();
   }
}

Board::Board(const Board &other)
: pieces{}, turn{ other.turn }, wKingIndex{ other.wKingIndex }, bKingIndex{ other.bKingIndex }, moveList{}
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
   // TODO:
   // Copy move history
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
      y[0] -= pieces[movingPieceIndex]->movementDirection;

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

void Board::checkTaking(const unsigned &movingPieceIndex, const string &from, const string &to, MoveRepresentation &move) {
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
      move[moveListPieceTakenIndex] = pieceToBeRemoveIndex;
      if (isEnPassant) {
         move[moveListIsEnPassant] = true;
      }
   }
}

void Board::checkPromoting(const unsigned &movingPieceIndex, const char &pieceToPromoteTo, MoveRepresentation &move) {
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
         move[moveListIsPromotion] = true;
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
      MoveRepresentation move;
      move[moveListMovingPieceIndex] = movingPieceIndex;
      move[moveListOldPos] = from;
      move[moveListNewPos] = to;
      move[moveListIsCastle] = false;
      move[moveListIsEnPassant] = false;
      move[moveListPieceTakenIndex] = 65u;
      move[moveListPieceHasMovedChanged] = false;
      move[moveListIsPromotion] = false;

      // Moved successfully
      moveSuccess = true;
      if (!pieces[movingPieceIndex]->hasMoved) {
         pieces[movingPieceIndex]->hasMoved = true;
         move[moveListPieceHasMovedChanged] = true;
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
         move[moveListIsCastle] = true;
      }
      // Disable en passant
      checkDisableEnPassant();

      // Check promoting
      checkPromoting(movingPieceIndex, pieceToPromoteTo, move);

      moveList.push_back(move);
   }
   return moveSuccess;
}

void Board::update() {
   for (auto& piece: pieces) {
      // No need to update all the pieces, only the ones to play
      if (piece->alive && piece->color == turn){
         piece->update();
      }
   }
   handlePins();
}

void Board::undoLastMove() {
   if (!moveList.empty()) {
      unsigned lastMoveIndex{ static_cast< unsigned >(moveList.size()) - 1u };
      unsigned movingPieceIndex{ std::get<unsigned>( moveList[lastMoveIndex][moveListMovingPieceIndex] ) };
      // Put piece in old position
      string oldPos{ std::get< string >( moveList[lastMoveIndex][moveListOldPos] ) };
      string newPos{ std::get< string >( moveList[lastMoveIndex][moveListNewPos] ) };
      bool isPromotion{ std::get< bool >( moveList[lastMoveIndex][moveListIsPromotion] ) };
      char type{ 'P' };
      // Handle taken pieces
      unsigned takenPieceIndex{ std::get<unsigned>( moveList[lastMoveIndex][moveListPieceTakenIndex] ) };
      if (takenPieceIndex != 65u) {
         pieces[takenPieceIndex]->alive = true;
      }
      // Type of pawn to if move was a promotion
      if (islower(pieces[movingPieceIndex]->type)) {
         type = 'p';
      }

      if (isPromotion) {
         pieces[movingPieceIndex].reset(new Pawn(&pieces, &turn, type, oldPos));
      }
      // If move was not a promotion
      else {
         pieces[movingPieceIndex]->pos = oldPos;
         // Check if the class member 'hasMove' changed
         bool hasMovedChanged{ std::get< bool >( moveList[lastMoveIndex][moveListPieceHasMovedChanged] ) };
         if (hasMovedChanged) {
            pieces[movingPieceIndex]->hasMoved = false;
         }
         // Handle castling
         bool isCastle{ std::get< bool >( moveList[lastMoveIndex][moveListIsCastle] ) };
         if (isCastle) {
            unsigned rookIndex{ 0u };
            string oldRookPos{ "XY" };
            oldRookPos[1] = oldPos[1];
            // Short castle
            if (newPos[0] == 'g') {
               rookIndex = pieces[movingPieceIndex]->shortCastleRookIndex;
               oldRookPos[0] = 'h';
            }
            // Long castle
            else {
               rookIndex = pieces[movingPieceIndex]->longCastleRookIndex;
               oldRookPos[0] = 'a';
            }
            pieces[rookIndex]->hasMoved = false;
            pieces[rookIndex]->pos = oldRookPos;
         }
         bool isEnPassant{ std::get< bool >( moveList[lastMoveIndex][moveListIsEnPassant] ) };
         if (isEnPassant) {
            // We already brought it back to the board
            pieces[takenPieceIndex]->enPassant = true;
         }
      }

      moveList.pop_back();
      changeTurn();
   }
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
   string x{ from[0] };
   string y{ from[1] };
   string temp{ x+y };
   while (temp != to) {
      temp[0] += direction[0];
      temp[1] += direction[1];

      moves.emplace_back(temp);
   }
   return moves;
}

void Board::handlePins() {
   constexpr int directions[8][2] = {
      {1, 0}, {-1, 0},
      {0, 1}, {0, -1},
      {1, 1}, {1, -1},
      {-1, 1}, {-1, -1}
   };
   unsigned kingIndex{ turn == WHITE ? wKingIndex : bKingIndex };
   string kingPos{ pieces[kingIndex]->pos };

   for (unsigned i=0u; i < 8u; i++) {
      string temp{ kingPos };
      temp[0] += directions[i][0];
      temp[1] += directions[i][1];

      bool checkingForPinnedPiece{ true };
      unsigned pinnedPieceIndex{ 1u };

      while (temp[0] != 'h'+1 && temp[0] != 'a'-1 && temp[1] != '0' && temp[1] != '9') {
         unsigned pieceIndex = getIndexOfPieceAt(temp);

         if (pieceIndex != 65u) {
            if (checkingForPinnedPiece) { // Looking for a piece to pin
               if (pieces[pieceIndex]->color == pieces[kingIndex]->color) {
                  pinnedPieceIndex = pieceIndex;
                  checkingForPinnedPiece = false;
               }
            }
            else { // Looking for the pinning piece
               if (pieces[pieceIndex]->color != pieces[kingIndex]->color) {
                  // The first 4 directions are horizontal and vertical
                  // The other 4 are diagonals
                  if (
                     tolower(pieces[pieceIndex]->type == 'q')
                     || (i < 4 && tolower(pieces[pieceIndex]->type) == 'r')
                     || (i >= 4 && tolower(pieces[pieceIndex]->type) == 'b' )
                  ) {
                     vector<string> legalSquares{
                        getAllSquaresInBetween(pieces[kingIndex]->pos, pieces[pieceIndex]->pos, directions[i])
                     };
                     removeMovesIfNotInVector(pinnedPieceIndex, legalSquares);
                     // cout << "pinned: "  << pieces[pinnedPieceIndex]->pos << ", by: " << pieces[pieceIndex]->pos << '\n';
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
