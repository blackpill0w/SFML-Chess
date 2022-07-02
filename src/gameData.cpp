#include "gameData.hpp"

//*******   PieceData   ********//

PieceData::PieceData()
: alive(true), pos("xy"), type(0), legalMoves() {

}
PieceData::PieceData( const unique_ptr<Chess::Piece> &piece )
: PieceData()
{
   store(piece);
}
void PieceData::store( const unique_ptr<Chess::Piece> &piece ) {
   legalMoves.clear();
   legalMoves.reserve(piece->legalMoves.size());

   alive = piece->alive;
   pos = piece->pos;
   type = piece->type;

   for (auto& move: piece->legalMoves) {
      legalMoves.emplace_back(move);
   }
}

//****** GameData ******//

GameData::GameData()
: gameState( Chess::MoveFailed ), turn( Chess::NONE ), piecesData()
{
}
GameData::GameData( const Chess::Board &board )
: GameData() {
   store(board);
}

void GameData::store( const Chess::Board &board ) {
   piecesData.clear();
   piecesData.reserve(board.pieces.size());

   gameState = board.gameState;
   turn = board.turn;

   for (auto& piece: board.pieces) {
      piecesData.emplace_back( PieceData( piece ) );
   }
}

//******* Operator overloading ******//

sf::Packet& operator<<(sf::Packet &packet, const PieceData &pieceData) {
   packet << pieceData.alive;
   packet << pieceData.pos;
   packet << static_cast<int>(pieceData.type);

   // Number of legal moves
   packet << static_cast<unsigned>(pieceData.legalMoves.size());
   for (auto& move: pieceData.legalMoves) {
      packet << move;
   }

   return packet;
}

sf::Packet& operator>>(sf::Packet &packet, PieceData &pieceData) {
   pieceData.legalMoves.clear();

   packet >> pieceData.alive;
   packet >> pieceData.pos;

   int iType{};
   packet >> iType;
   pieceData.type = static_cast<char>(iType);

   unsigned legalMovesSize{};
   packet >> legalMovesSize;

   string pos;
   for (unsigned i=0u; i < legalMovesSize; i++) {
      packet >> pos;
      pieceData.legalMoves.emplace_back(pos);
   }
   return packet;
}

sf::Packet& operator<<(sf::Packet &packet, const GameData &gameData) {
   packet << static_cast<unsigned>(gameData.gameState);
   packet << static_cast<unsigned>(gameData.turn);

   // Number of pieces
   packet << static_cast<unsigned>(gameData.piecesData.size());
   for (auto& pieceData: gameData.piecesData) {
      packet << pieceData;
   }
   return packet;
}

sf::Packet& operator>>(sf::Packet &packet, GameData &gameData) {
   gameData.piecesData.clear();

   unsigned uTemp{};
   packet >> uTemp;
   gameData.gameState = Chess::GameState( uTemp );

   packet >> uTemp;
   gameData.turn = Chess::Turn( uTemp );

   unsigned nPieces{};
   packet >> nPieces;

   PieceData temp;
   for (unsigned i=0u; i < nPieces; i++) {
      packet >> temp;
      gameData.piecesData.emplace_back(temp);
   }
   return packet;
}
