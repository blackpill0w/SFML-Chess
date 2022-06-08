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

   Piece(const Piece& other);
   Piece operator=(const Piece& other) = delete;
   virtual ~Piece() {};

   PieceColor isPosOccupied(const string &pos);
   bool isValidMove(const string &move);
   bool canPosBeAttacked(const string &pos, const PieceColor &color);
   // Will be overriden for pawn (and possibly king)
   virtual void update();
   // For kings
   virtual void shortCastle();
   virtual void longCastle();
protected:
   virtual void setLegalMoves() = 0;

};

#endif