#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>

#include "pieces.hpp"

using std::cout;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;
using std::find;

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
public:
   Board();
   //********************************************
   //**
   //** Moving a piece and everything that comes with that
   //** the piece to be moved is deduced from the 'from' var
   //**
   //********************************************
   bool move(string &from, string &to, const char &pieceToPromoteTo = 0);

   //********************************************
   //**
   //** Get index (in the vector of pieces) of a piece
   //** from a position (and optionally the
   //** opposite color of the piece).
   //**
   //********************************************
   unsigned getIndexOfPieceAt(const string &pos, const PieceColor &colorToBeDifferentFrom = NONE);

   //********************************************
   //**
   //** Update the state of the board.
   //**
   //********************************************
   void update();

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
   void removePiece(unsigned &index);

   //********************************************
   //**
   //** Handle castling.
   //**
   //********************************************
   bool isMoveCastle(unsigned &movingPieceIndex, string &from, string &to);

   void checkEnableEnPassant(unsigned &movingPieceIndex, string &from, string &to);

   unsigned checkIfMoveWasEnPassant(unsigned &movingPieceIndex, string &from, string &to);

   void checkTaking(unsigned &movingPieceIndex, string &from, string &to);

   void checkDisableEnPassant();

   void checkPromoting(unsigned &movingPieceIndex, const char &pieceToPromoteTo);
};

#endif