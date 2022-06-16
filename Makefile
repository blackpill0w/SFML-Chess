CXX = g++
CXXFLAGS = -std=c++14 -Wall -Weffc++ -Wextra -Wsign-conversion -g
LDFLAGS = -l sfml-window -l sfml-system -l sfml-graphics

SRC=src
CHESS=Chess

chess: pieceSprite.o utils.o piece.o king.o queen.o rook.o bishop.o knight.o pawn.o board.o game.o main.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} $^ -o $@
main.o: ${SRC}/main.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/main.cpp
game.o: ${SRC}/game.cpp ${SRC}/game.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/game.cpp
pieceSprite.o: ${SRC}/pieceSprite.cpp ${SRC}/pieceSprite.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/pieceSprite.cpp
utils.o: ${SRC}/utils.hpp ${SRC}/utils.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/utils.cpp
board.o: ${SRC}/${CHESS}/board.hpp ${SRC}/${CHESS}/board.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/board.cpp
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
piece.o: ${SRC}/${CHESS}/piece.hpp ${SRC}/${CHESS}/piece.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/${CHESS}/piece.cpp

clean:
	rm -rf *.o chess
