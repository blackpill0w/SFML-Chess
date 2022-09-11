#include "board.hpp"

#include <string>
#include <sstream>
#include <iostream>

using std::cerr;
using std::make_unique;
using std::string;

namespace Chess {
	void Board::loadFEN(const string &fenStr) {
		if ( !pieces.empty() ) {
			pieces.clear();
		}

      std::istringstream fen{ fenStr.c_str() };
      string toks[5]; // 100 rule not implemented

		enum : unsigned {
         TOK_PIECES_POS = 0u,
         TOK_TURN,
         TOK_CASTLING_RIGHTS,
         TOK_ENPASSANT_POS,
         TOK_FIFTY_MOVE_RULE
      };

      for (auto& tok: toks) {
         std::getline(fen, tok, ' ');
      }
      // Check if parsing failed
      for (auto& tok: toks) {
         if (tok.empty()) {
            cerr << "Error: couldn't parse FEN String, exiting...\n";
            cerr << "FEN string: " << fen.str() << '\n';
            exit(1);
         }
      }
      loadPieces(toks[TOK_PIECES_POS]);

		if (toks[TOK_TURN] == "b") { // turn is white by default
         turn = BLACK;
      }

      setCastlingRights(toks[TOK_CASTLING_RIGHTS]);

      setEnpassantTargetSquare(toks[TOK_ENPASSANT_POS]);

      fiftyMoveRuleCounter = std::stoi(toks[TOK_FIFTY_MOVE_RULE]);

	} // loadFEN
	void Board::loadPieces(const string &FENpiecesStr) {
		string x{ "a" };
		string y{ "8" };
		for (auto c: FENpiecesStr) {
			if (c == '/') {
				x = "a";
				y[0] -= 1;
			}
			else if (isdigit(c)) {
				x[0] += ( (c - '1') + 1 );
			}
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
               cerr << "Error: couldn't parse FEN string.\n";
					cerr << "When loading pieces, from: " << FENpiecesStr << '\n';
					cerr << "Invalid character: "<< c << '\n';
					cerr << "Exiting...\n";
					exit(1);
				}
			}
		}
	} // loadPieces()

   void Board::setCastlingRights(const string &FENcastlingRights) {
      if (FENcastlingRights == "-") {
         return;
      }
      for (auto c: FENcastlingRights) {
         if (c == 'K') {
            pieces[wKingIndex]->canShortCastle = true;
         }
         else if (c == 'Q') {
            pieces[wKingIndex]->canLongCastle = true;
         }
         else if (c == 'k') {
            pieces[bKingIndex]->canShortCastle = true;
         }
         else if (c == 'q') {
            pieces[bKingIndex]->canLongCastle = true;
         }
      }

   } // setCastlingrights()

   void Board::setEnpassantTargetSquare(const string &FENenpassantSquare) {
      if (FENenpassantSquare == "-") {
         return;
      }
      int pawnMovementDir = 1;
      if (turn == WHITE) { // if turn is white meaning black will be enpassant-ed
         pawnMovementDir = -1;
      }
      string enpassantPiecePos{ "xx" };
      enpassantPiecePos[0] = FENenpassantSquare[0];
      enpassantPiecePos[0] = FENenpassantSquare[1] + pawnMovementDir;
      unsigned pieceI{ getIndexOfPieceAt(enpassantPiecePos) };
      if (pieceI == 65u) {
         cerr << "Error: Failed to parse enpassant target square\n";
         cerr << "Reason: No piece is at the target square.\n";
         exit(1);
      }
      if (tolower(pieces[pieceI]->type) != 'p') {
         cerr << "Error: Failed to parse enpassant target square\n";
         cerr << "Reason: pieces at the target square is not a pawn.\n";
         exit(1);
      }
      pieces[pieceI]->enPassant = true;

   }

} // namespace Chess
