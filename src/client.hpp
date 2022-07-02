#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <SFML/Network.hpp>

#include "Chess/board.hpp"
#include "Utils/utils.hpp"
#include "gameData.hpp"

void startClient();

void waitBoardUpdate(sf::TcpSocket &client, sf::Packet &dataPacket, GameData &gameData);

#endif
