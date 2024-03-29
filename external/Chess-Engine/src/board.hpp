#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <memory>

#include "pieces.hpp"

using std::string;
using std::vector;
using std::unique_ptr;

namespace Chess
{
/*! TODO
  A class to store a game's state.
*/
enum GameState : unsigned {
   MoveFailed, Moved,
   MoveAndCapture, MoveAndCastle,
   Check, CheckMate, Draw
};

/*! TODO
  A class to store a move's data.
*/
class MoveData {
public:
  string oldPos{};
  string newPos{};
  unsigned movingPieceIndex{ 65u };
  unsigned takenPieceIndex{ 65u };
  bool isMoveEnPassant{ false };
  bool isMoveCastle{ false };
  bool pieceHasMovedChanged{ false };
  bool isPromotion{ false };
  unsigned previousFiftyMoveRule{ 0 };
public:
bool operator==(const MoveData &other) const {
   return (
      oldPos == other.oldPos
      && newPos == other.newPos
      && movingPieceIndex == other.movingPieceIndex
      && takenPieceIndex == other.takenPieceIndex
      && isMoveEnPassant == other.isMoveEnPassant
      && isMoveCastle == other.isMoveCastle
      && pieceHasMovedChanged == other.pieceHasMovedChanged
      && isPromotion == other.isPromotion
   );
}
};


/*!
 A class to handle the game's logic,
 the representation of the board is a vector of unique_ptr
 of the 'Piece' class.
*/

class Board {

public:

   //! The representation of the board.
   vector< unique_ptr<Piece> > pieces;

   //! Color of the pieces to play.
   Turn turn;

   //! Game's state.
   GameState gameState;
protected:
   //! Indices of the kings
   unsigned wKingIndex;
   unsigned bKingIndex;
   /*!
     It is used to save data about moves in order
     to implement the undo functionality.
   */
   vector< MoveData > moveList;

   //! The number of possible moves.
   unsigned possibleMoves;

   //! Number of pieces at the beginning of the game.
   unsigned initialPiecesNum;

   //! Counter for the fifty move rule.
   unsigned fiftyMoveRuleCounter;
public:
   /*!
     The constructor, takes an optional
     parameter to load an FEN string.
     @param fenStr the FEN string, initial state of the board.
   */
   Board(const string &fenStr = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

   /*!
     Functions to load an FEN string.
     @param fenStr the FEN string.
   */
   void loadFEN(const string &fenStr);
   /*!
     Load the pieces from an FEN string.
     @param FENpiecesStr positions of the pieces from FEN string.
   */
	void loadPieces(const string &FENpiecesStr);
   /*!
    Set castling rights from an FEN string.
    @param FENcastlingRights castling rights from an FEN string.
   */
   void setCastlingRights(const string &FENcastlingRights);
   /*!
     Set enpassant from an FEN string.
     @param FENenpassantSquare enpassant square from an FEN string.
   */
   void setEnpassantTargetSquare(const string &FENenpassantSquare);

   /*!
     @brief Move a piece.
     @param from position of the piece to move.
     @param to position to move the piece to
     @param pieceToPromoteto piece to promote to if the move is a promotion,
            optional if the move is not a promotion.
   */
   void move(const string &from, const string &to, const char pieceToPromoteTo = 0);

   /*!
     @brief Get index (in the vector of pieces) of a piece.
     @param pos position of the piece
     @param colorToBeDifferentFrom (optional) the color of the enemy's pieces.
     @param alivestate (optional) whether the piece is on the board
            (if a piece is taken it stays in the vector, but in a dead state).
     @return index of the piece if it was found, otherwise 65.
   */
   unsigned getIndexOfPieceAt(const string &pos, const PieceColor &colorToBeDifferentFrom = NONE, const bool &aliveState = true);

   //! Update the state of the board.
   void update();

   //! Undo the last move.
   void undoLastMove();

   //! Get the number of possible moves.
   unsigned getNumberOfPossibleMoves();

   //! Put board in initial state (last FEN string passed).
   void reset();

protected:
   //! Change the turn.
   void changeTurn();

   //! Remove a piece from the board.
   void removePiece(const unsigned &index);

   //! Check if the move made was castle.
   bool isMoveCastle(const unsigned &movingPieceIndex, const string &from, const string &to);

   //! Handle castling.
   void handleCastling(const unsigned &movingPieceIndex, const string &from, const string &to);

   //! Check if a pawn can be en-passant-ed.
   void checkEnableEnPassant(const unsigned &movingPieceIndex, const string &from, const string &to);

   //! Check if the move made was en passant.
   unsigned checkIfMoveWasEnPassant(const unsigned &movingPieceIndex, const string &from, const string &to);

   //! Check if the move made was a capture.
   void checkTaking(const unsigned &movingPieceIndex, const string &from, const string &to, MoveData &move);

   /*!
     Check if en passant was legal for some pawn
     and make it illegal.
   */
   void checkDisableEnPassant();

   //! Check if the move made was a pawn promotion.
   void checkPromoting(const unsigned &movingPieceIndex, const char pieceToPromoteTo, MoveData &move);

   /*!
     Gives all the squares between two positions (inclusif)
     moving in a given direction.
   */
   vector< string > getAllSquaresInBetween(string &from, string &to, const int direction[2]);

   /*!
     Given a vector of positions and an index of a piece,
     every piece's possible move that is not in that vector
     is removed.
   */
   void removeMovesIfNotInVector(const unsigned &pieceIndex, const vector< string > &moves);

   //! Handle pins.
   void handlePins();

   //! Handle checks.
   void handleChecks();

   /*!
     Handle checks of sliding pieces
     (queens, rooks and bishops).
   */
   void handleSlidingPiecesChecks();

   //! Handle checks of pawns and knights.
   void handlePawnKnightCheck();

   //! Calculate the number of possible moves.
   void calculatePossibleMoves();

   //! Checks for draw by repitition.
   bool isDrawByRepitition();

   //! Checks if there is a checkmate or a draw.
   void checkGameEnd();
}; // Board class

} // Chess namespace

#endif
