#include "./server.hpp"
#include "./gameData.hpp"
#include "./utils.hpp"
#include <iostream>

using std::cout;
using std::cerr;
using std::string;

int main() {
   startServer();
}

void startServer() {
   cout << "Starting the server...\n";
   sf::TcpListener   listener;
   sf::Packet        dataPacket;
   listener.listen(utils::port);

   cout << "Server listening...\n";

   sf::TcpSocket  wPlayer;
   sf::TcpSocket  bPlayer;

   listener.accept(wPlayer);
   cout << "The first player is connected!\n";
   sendData(wPlayer, dataPacket, string("white"));

   listener.accept(bPlayer);
   cout << "The second player is connected!\n";
   sendData(bPlayer, dataPacket, string("black"));

   cout << "Starting the game...\n";
   Chess::Board board;

   sf::TcpSocket     *playerToPlay{ nullptr };
   GameData          gameData;

   wPlayer.setBlocking(false);
   bPlayer.setBlocking(false);

   while (true) {
      gameData.store( board );

      // Send board data
      cout << "Sending data...\n";
      if (sendData(wPlayer, dataPacket, gameData) == sf::Socket::Disconnected) {
         break;
      }
      sf::Socket::Status s =  sendData(bPlayer, dataPacket, gameData);
      if (s == sf::Socket::Disconnected) {
         break;
      }
      else if (s == sf::Socket::Error) {
         cerr << "Error\n";
      }
      else if (s == sf::Socket::NotReady) {
         cout << "Not ready\n";
      }

      playerToPlay = board.turn == Chess::WHITE ? &wPlayer : &bPlayer;

      cout << "Waiting for player to move...\n";
      if ( receiveAndPlayMove(*playerToPlay, board, dataPacket) == sf::Socket::Disconnected) {
         break;
      }
      cout << "Received move\n";
   }

   cout << "Shutting down...\n";
   wPlayer.disconnect();
   bPlayer.disconnect();
   listener.close();
}

template<typename T>
sf::Socket::Status sendData(sf::TcpSocket &receiver, sf::Packet &dataPacket, const T &data) {
   dataPacket << data;
   sf::Socket::Status status{ receiver.send(dataPacket) };
   dataPacket.clear();

   return status;
}

sf::Socket::Status receiveAndPlayMove(sf::TcpSocket &player, Chess::Board &board, sf::Packet &dataPacket) {
   string   from{};
   string   to{};
   int      iPieceToPromoteTo{};
   char     pieceToPromoteTo{};
   sf::Socket::Status status;


   while (true) {
     status = player.receive(dataPacket);

     if (status == sf::Socket::Done) {
       dataPacket >> from >> to >> iPieceToPromoteTo;

       pieceToPromoteTo = iPieceToPromoteTo;

       board.move(from, to, pieceToPromoteTo);

       dataPacket.clear();

       break;
     }
     else if (status == sf::Socket::Disconnected) {
       // Return status
       break;
     }
   }
   return status;
}
