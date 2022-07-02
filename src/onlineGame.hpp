#ifndef ONLINE_GAME_HPP
#define ONLINE_GAME_HPP

#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "onlinePieceSprite.hpp"
#include "gameData.hpp"
#include "Utils/utils.hpp"

using std::vector;
using std::string;

void onlineGame(sf::RenderWindow &window);

void updateSprites(const GameData &gameData, vector<OnlinePieceSprite> &sprites);

void checkBoardUpdate(sf::TcpSocket &client, vector<OnlinePieceSprite> &sprites, sf::Packet &dataPacket, GameData &gameData);

void sendMove(sf::TcpSocket &client, const string &from, const string &to, const char pieceToPromoteTo, sf::Packet &dataPacket);

#endif
