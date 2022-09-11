#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector>
#include <memory>

using std::unique_ptr;
using std::vector;
using std::string;

namespace Chess
{

enum PieceColor : unsigned { NONE = 0u, BLACK, WHITE };
using Turn = PieceColor;

class Piece {

public:
   //! Type of the piece
   const char     type;
   //! Color of the piece.
   PieceColor     color;
   //! Whether the piece on the board.
   bool           alive;
   //! Position of the piece.
   string         pos;
   //! Whether the piece has moved.
   bool           hasMoved;
   //! A list of legal moves.
   vector<string> legalMoves;
   /*!
     A pointer to the list of pieces (from the Board class).
   */
   const vector< unique_ptr<Piece> > *pieces;
   //! A pointer the color to play (from the Board class).
   const Turn     *turn;
   //! Whether the pieces is protected by another piece.
   bool           isProtected;
   //! Initial position of the piece.
   const string   initialPos;
   //! Position a piece can attack (used with Pawns only)
   string         attackingPositions[2];
   //! Whether the can be enpassant-ed (used with Pawns only)
   bool           enPassant;
   /*!
     Movement direction of the piece (used with Pawns only).
     1 for white and -1 for black.
   */
   int            pawnMovementDirection;
   //! Index of the kind side rook (from the vector of pieces), used with Kings only.
   unsigned  shortCastleRookIndex{ 65u };
   //! Index of the queen side rook (from the vector of pieces), used with Kings only.
   unsigned  longCastleRookIndex{ 65u };
   //! Whether the king is in check, used with Kings only.
   bool      inCheck;
   //! Whether the king can castle king side, used with Kings only.
	bool      canShortCastle;
   //! Whether the king can castle queen side, used with Kings only.
	bool      canLongCastle;
public:
   /*!
     The constructor.
     @param pieces a pointer to the vector of pieces (from the Board class).
     @see Chess::Board
     @param turn a pointer to the turn class member of the containing Board class.
     @param pos the initial position of the piece.
   */
   Piece(const vector< unique_ptr<Piece> > *pieces, Turn *turn, const char type, const string &pos);

   // Because of the pointer class memeber
   Piece(const Piece& other) = default;
   Piece operator=(const Piece& other) = delete;
   virtual ~Piece() {};

   /*!
     Check wether a position is occupied.
     @return PieceColor object, the color of the piece occupying the positon.
     @return PiecesColor::NONE if no pieces os at the given position.
   */
   PieceColor isPosOccupied(const string &pos);

   /*!
     @brief Checks wether a move is valid.
     @param move the position to check.
     @return true if the move is valid, false otherwise.
   */
   bool isValidMove(const string &move);

   /*!
     @brief Check whether a position can be attacked.
     @param pos the position to be tested.
     @param the color of the piece to test relative to.
     @return true is position can be attacked, false otherwise.
   */
   bool canPosBeAttacked(const string &pos, const PieceColor &color);

   //! Return to initial state (as if the piece was just created).
   void reset();

   //! Overridden for 'Pawn'
   virtual void update();

   //! Handles king side castling, Kings only.
   virtual void shortCastle();
   //! Handles queen side castling, Kings only.
   virtual void longCastle();
   //! Save rooks indices, Kings only.
   virtual void setRooksIndex();
protected:
   //! Calculates the legal moves.
   virtual void setLegalMoves();
   //! Removes moves outside board (ex: e9...).
   void removeMovesOutsideBoard();

};

} // Chess namespace

#endif
