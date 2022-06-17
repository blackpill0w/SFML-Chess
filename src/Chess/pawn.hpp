#ifndef PAWN_HPP
#define PAWN_HPP

#include "piece.hpp"

namespace Chess
{

class Pawn : public Piece {
public:
   Pawn(const vector< unique_ptr<Piece> > *pieces, PieceColor *turn, const char &type, const string &pos);
   void setLegalMoves() override;
   void update() override;
protected:
   //********************************************
   //**
   //** Update the attacking positions of a pawn.
   //**
   //********************************************
   void updateAttackingPositions();

   //********************************************
   //**
   //** Check if en passant is possible.
   //**
   //********************************************
   void checkEnPassant();

   //********************************************
   //**
   //** Check if en passant results in a check,
   //** or rather, in taking the king.
   //**
   //********************************************
   bool isEnPassantResultsInCheck();
};

}

#endif
