#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <SFML/Network.hpp>

#include "Chess/board.hpp"
#include "gameData.hpp"
#include "Utils/utils.hpp"

void startServer();

template<typename T>
sf::Socket::Status sendData(sf::TcpSocket &receiver, sf::Packet &dataPacket, const T &data);

sf::Socket::Status receiveAndPlayMove(sf::TcpSocket &player, Chess::Board &board, sf::Packet &dataPacket);

#endif
