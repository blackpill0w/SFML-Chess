#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/Network.hpp>

#include "./board.hpp"

void startServer();

template<typename T>
sf::Socket::Status sendData(sf::TcpSocket &receiver, sf::Packet &dataPacket, const T &data);

sf::Socket::Status receiveAndPlayMove(sf::TcpSocket &player, Chess::Board &board, sf::Packet &dataPacket);

#endif
