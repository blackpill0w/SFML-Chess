#ifndef PIECE_HPP
#define PIECE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

using std::unique_ptr;
using std::vector;
using std::string;
using std::find;
using std::cout;

namespace Chess
{

enum PieceColor : unsigned {NONE = 0u, BLACK, WHITE};
using Turn = PieceColor;

class Piece {

public:
   const char type;
   PieceColor color;
   bool alive;
   string pos;
   bool hasMoved;
   vector<string> legalMoves;
   const vector< unique_ptr<Piece> > *pieces;
   const Turn *turn;
   bool isProtected;
   const string initialPos;
   // NOTE: Only for pawns
   string attackingPositions[2];
   bool enPassant;
   int pawnMovementDirection;
   // NOTE: Only for king
   unsigned shortCastleRookIndex{ 65u };
   unsigned longCastleRookIndex{ 65u };
   bool inCheck;
public:
   Piece(const vector< unique_ptr<Piece> > *pieces, Turn *turn, const char type, const string &pos);


   // Just to make the compiler's warnings disappear
   Piece(const Piece& other);
   Piece operator=(const Piece& other) = delete;
   virtual ~Piece() {};

   //********************************************
   //**
   //** Checks wether a position is occupied.
   //** It returns a 'PieceColor' object.
   //** A position is not occupied if the return value
   //** if PieceColor::NONE.
   //**
   //********************************************
   PieceColor isPosOccupied(const string &pos);

   //********************************************
   //**
   //** Checks wether a move is valid.
   //** A move is valid if it's in the 'legalMoves'
   //** vector.
   //**
   //********************************************
   bool isValidMove(const string &move);

   //********************************************
   //**
   //** Given a position and the color of
   //** the piece (that might be attacked),
   //** it returns wether that position can be attacked.
   //**
   //********************************************
   bool canPosBeAttacked(const string &pos, const PieceColor &color);

   //********************************************
   //**
   //** Return to initial state
   //**
   //********************************************
   void reset();

   // Overridden for 'Pawn' and 'King'
   virtual void update();

   //** For kings
   //**
   //********************************************
   //**
   //** Handles castling.
   //**
   //********************************************
   virtual void shortCastle();
   virtual void longCastle();
   //********************************************
   //**
   //** Save rooks indices, helps with castling.
   //**
   //********************************************
   virtual void setRooksIndex();
protected:
   //********************************************
   //**
   //** Puts the legal moves in the 'legalMoves'
   //** vector.
   //**
   //********************************************
   virtual void setLegalMoves();
   //********************************************
   //**
   //** Removes moves outside board (ex: e9...).
   //**
   //********************************************
   void removeMovesOutsideBoard();

};

} // Chess namespace

#endif
