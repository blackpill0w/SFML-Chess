#ifndef PIECE_HPP
#define PIECE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstring>

using std::unique_ptr;
using std::vector;
using std::string;
using std::find;
using std::cout;

enum PieceColor {NONE = 0, BLACK, WHITE};

class Piece {

public:
   const char type;
   PieceColor color;
   bool alive;
   string pos;
   bool hasMoved;
   vector<string> legalMoves;
   const vector< unique_ptr<Piece> > *pieces;
   const PieceColor *turn;
   // WARNING: Only for pawns
   string attackingPositions[2];
   bool enPassant;
   int movementDirection;
public:
   Piece(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos);


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

   // Overridden for 'Pawn' and 'King'
   virtual void update();

   //** For kings
   //********************************************
   //**
   //** Handles castling.
   //**
   //********************************************
   virtual void shortCastle();
   virtual void longCastle();
protected:
   //********************************************
   //**
   //** Puts the legal moves in the 'legalMoves'
   //** vector.
   //**
   //********************************************
   virtual void setLegalMoves() = 0;

};

#endif