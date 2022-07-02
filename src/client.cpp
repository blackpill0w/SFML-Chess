#include "client.hpp"

using std::cout;
using std::cin;

int main() {
   startClient();
}

void startClient() {
   sf::TcpSocket client;
   sf::Packet dataPacket;
   GameData gameData;

   cout << "Starting...\n";
   cout << "Connecting...\n";
   client.connect("localhost", utils::port);
   cout << "Connected!\n";

   string color{};
   Chess::PieceColor myColor{ Chess::WHITE };

   client.receive(dataPacket);
   dataPacket >> color;
   if (color == "black") {
      myColor = Chess::BLACK;
   }

   string from{};
   string to{};

   while (from != "q" && to != "q") {
      if (gameData.turn == myColor) {
         cout << "From: ";
         cin >> from;
         cout << "To: ";
         cin >> to;

         dataPacket << from << to << static_cast<int>('q');
         client.send( dataPacket );
         dataPacket.clear();

         cout << "Waiting server's respond...\n";
         waitBoardUpdate(client, dataPacket, gameData);
      }
      else {
         cout << "Waiting for other player...\n";
         waitBoardUpdate(client, dataPacket, gameData);
      }
   }
}

void waitBoardUpdate(sf::TcpSocket &client, sf::Packet &dataPacket, GameData &gameData) {
   client.receive(dataPacket);
   dataPacket >> gameData;
   dataPacket.clear();
}

