CXX = g++
CXXFLAGS = -std=c++14 -Wall -Weffc++ -Wextra -Wsign-conversion -g
LDFLAGS = -l sfml-window -l sfml-system -l sfml-audio -l sfml-graphics -l sfml-network

SRC=src
CHESS=Chess
UTILS=Utils

default:
	make chess
	make server

main.o: ${SRC}/main.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/main.cpp

# Utils
COMPILED_UTILS = sfmlColors.o button.o utils.o
utils.o: ${SRC}/${UTILS}/utils.hpp ${SRC}/${UTILS}/utils.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${UTILS}/utils.cpp
button.o: ${SRC}/${UTILS}/button.hpp ${SRC}/${UTILS}/button.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${UTILS}/button.cpp
sfmlColors.o: ${SRC}/${UTILS}/sfmlColors.hpp ${SRC}/${UTILS}/sfmlColors.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${UTILS}/sfmlColors.cpp

app.o: ${SRC}/app.cpp ${SRC}/app.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/app.cpp
pieceSprite.o: ${SRC}/pieceSprite.cpp ${SRC}/pieceSprite.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/pieceSprite.cpp
offlineGame.o: ${SRC}/offlineGame.cpp ${SRC}/offlineGame.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/offlineGame.cpp
onlinePieceSprite.o: ${SRC}/onlinePieceSprite.cpp ${SRC}/onlinePieceSprite.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/onlinePieceSprite.cpp
onlineGame.o: ${SRC}/onlineGame.cpp ${SRC}/onlineGame.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/onlineGame.cpp
gameData.o: ${SRC}/gameData.cpp ${SRC}/gameData.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/gameData.cpp

server.o: ${SRC}/server.cpp ${SRC}/server.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/server.cpp

# Chess
COMPILED_CHESS = piece.o king.o queen.o rook.o bishop.o knight.o pawn.o board.o
board.o: ${SRC}/${CHESS}/board.hpp ${SRC}/${CHESS}/board.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/board.cpp
piece.o: ${SRC}/${CHESS}/piece.hpp ${SRC}/${CHESS}/piece.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/piece.cpp
king.o: ${SRC}/${CHESS}/king.hpp ${SRC}/${CHESS}/king.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/king.cpp
queen.o: ${SRC}/${CHESS}/queen.hpp ${SRC}/${CHESS}/queen.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/queen.cpp
rook.o: ${SRC}/${CHESS}/rook.hpp ${SRC}/${CHESS}/rook.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/rook.cpp
bishop.o: ${SRC}/${CHESS}/bishop.hpp ${SRC}/${CHESS}/bishop.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/bishop.cpp
knight.o: ${SRC}/${CHESS}/knight.hpp ${SRC}/${CHESS}/knight.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/knight.cpp
pawn.o: ${SRC}/${CHESS}/pawn.hpp ${SRC}/${CHESS}/pawn.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/pawn.cpp


chess: ${COMPILED_UTILS} ${COMPILED_CHESS} pieceSprite.o offlineGame.o gameData.o onlinePieceSprite.o onlineGame.o app.o main.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} $^ -o $@

server: utils.o server.o gameData.o ${COMPILED_CHESS}
	${CXX} ${LDFLAGS} ${CXXFLAGS} $^ -o $@



clean:
	rm -rf *.o chess server

