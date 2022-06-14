#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>
#include <variant>
#include <array>

#include "pieces.hpp"

using std::cout;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;
using std::array;
using std::find;

using MoveRepresentation = array< std::variant<unsigned, bool, string>, 8 >;

// Enum to easily access elements in moveList class member
enum : unsigned int {
   moveListMovingPieceIndex = 0u,
   moveListOldPos, moveListNewPos,
   moveListPieceTakenIndex,
   moveListIsEnPassant, moveListIsCastle,
   moveListPieceHasMovedChanged,
   moveListIsPromotion
};

//**
//********************************************
//**
//** A class to handle the game's logic,
//** the board is a vector of unique_ptr
//** to the 'Piece' class (abstract class).
//**
//** This class is completely seperate from
//** the gui.
//**
//********************************************

class Board {

public:
   //********************************************
   //**
   //** The representation of the board.
   //**
   //********************************************
   vector< unique_ptr<Piece> > pieces;

   //********************************************
   //**
   //** Color of the pieces to play.
   //**
   //********************************************
   PieceColor turn;
protected:
   //********************************************
   //**
   //** Indices of the kings
   //**
   //********************************************
   unsigned wKingIndex;
   unsigned bKingIndex;
   //********************************************
   //**
   //** A vector of arrays (8 elements, see the enum above)
   //** containing variants (unsinged, bool, string).
   //** It is used to save moves and to undo them.
   //**
   //********************************************
   vector< MoveRepresentation > moveList;
public:
   //********************************************
   //**
   //** The constructor, takes an optional
   //** parameter to load an FEN string.
   //**
   //********************************************
   Board(const string &fenStr = "");

   //********************************************
   //**
   //** The copy constructor.
   //**
   //********************************************
   Board(const Board &other);

   //********************************************
   //**
   //** Loads an FEN string.
   //**
   //********************************************
   void loadFEN(const string &fenStr);

   //********************************************
   //**
   //** Moving a piece and everything that comes with that
   //** the piece to be moved is deduced from the 'from' var.
   //**
   //********************************************
   bool move(const string &from, const string &to, const char &pieceToPromoteTo = 0);

   //********************************************
   //**
   //** Get index (in the vector of pieces) of a piece
   //** from a position (and optionally the
   //** opposite color of the piece).
   //**
   //********************************************
   unsigned getIndexOfPieceAt(const string &pos, const PieceColor &colorToBeDifferentFrom = NONE, const bool &aliveState = true);

   //********************************************
   //**
   //** Update the state of the board.
   //**
   //********************************************
   void update();

   //********************************************
   //**
   //** Simple, undo the last move.
   //**
   //********************************************
   void undoLastMove();

protected:
   //********************************************
   //**
   //** Simple. Change the turn.
   //**
   //********************************************
   void changeTurn();

   //********************************************
   //**
   //** Remove a piece from the board (taking a piece).
   //**
   //********************************************
   void removePiece(const unsigned &index);

   //********************************************
   //**
   //** Check if the move made was castle.
   //**
   //********************************************
   bool isMoveCastle(const unsigned &movingPieceIndex, const string &from, const string &to);

   //********************************************
   //**
   //** Handle castling.
   //**
   //********************************************
   void handleCastling(const unsigned &movingPieceIndex, const string &from, const string &to);

   //********************************************
   //**
   //** Check if a pawn can be en-passant-ed.
   //**
   //********************************************
   void checkEnableEnPassant(const unsigned &movingPieceIndex, const string &from, const string &to);

   //********************************************
   //**
   //** Check if the move made was en passant.
   //**
   //********************************************
   unsigned checkIfMoveWasEnPassant(const unsigned &movingPieceIndex, const string &from, const string &to);

   //********************************************
   //**
   //** Check if the move made was a capture.
   //**
   //********************************************
   void checkTaking(const unsigned &movingPieceIndex, const string &from, const string &to, MoveRepresentation &move);

   //********************************************
   //**
   //** Check if en passant was legal for some pawn
   //** and make it illegal.
   //**
   //********************************************
   void checkDisableEnPassant();

   //********************************************
   //**
   //** Check if the move made was a pawn promotion.
   //**
   //********************************************
   void checkPromoting(const unsigned &movingPieceIndex, const char &pieceToPromoteTo, MoveRepresentation &move);

   //********************************************
   //**
   //** Gives all the squares between two positions (inclusif)
   //** moving in a given direction.
   //**
   //********************************************
   vector< string > getAllSquaresInBetween(string &from, string &to, const int direction[2]);

   //********************************************
   //**
   //** Given a vector of positions and an index of a piece,
   //** every piece's possible move that is not in that vector
   //** is removed.
   //**
   //********************************************
   void removeMovesIfNotInVector(const unsigned &pieceIndex, const vector< string > &moves);

   //********************************************
   //**
   //** Handle pins.
   //**
   //********************************************
   void handlePins();
};

#endif