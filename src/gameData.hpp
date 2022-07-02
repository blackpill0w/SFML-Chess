#ifndef GAME_DATA_HPP
#define GAME_DATA_HPP

#include <memory>
#include <SFML/Network.hpp>

#include "Chess/board.hpp"
#include "Utils/utils.hpp"

using std::string;
using std::unique_ptr;

class PieceData {
public:
   bool alive{ true };
   string pos{};
   char type{ 0 };
   vector< string > legalMoves{};
public:
   PieceData();
   PieceData( const unique_ptr<Chess::Piece> &piece );
   void store( const unique_ptr<Chess::Piece> &piece);
};

class GameData {
public:
   Chess::GameState gameState{};
   Chess::Turn turn{};
   vector< PieceData > piecesData{};
public:
   GameData();
   GameData( const Chess::Board &board );

   void store( const Chess::Board &board );
};

sf::Packet& operator<<(sf::Packet &packet, const PieceData &pieceData);
sf::Packet& operator>>(sf::Packet &packet, PieceData &pieceData);

sf::Packet& operator<<(sf::Packet &packet, const GameData &gameData);
sf::Packet& operator>>(sf::Packet &packet, GameData &gameData);

#endif

