CXX = g++
CXXFLAGS = -Wall -Weffc++ -Wextra -Wsign-conversion -g
LDFLAGS = -l sfml-window -l sfml-system -l sfml-graphics

SRC=src

chess: pieceSprite.o utils.o piece.o king.o queen.o rook.o bishop.o knight.o pawn.o board.o main.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} $^ -o $@
main.o: ${SRC}/main.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/main.cpp
pieceSprite.o: ${SRC}/pieceSprite.cpp ${SRC}/pieceSprite.hpp
	${CXX} ${CXXFLAGS} -c ${SRC}/pieceSprite.cpp
board.o: ${SRC}/board.hpp ${SRC}/board.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/board.cpp
king.o: ${SRC}/king.hpp ${SRC}/king.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/king.cpp
queen.o: ${SRC}/queen.hpp ${SRC}/queen.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/queen.cpp
rook.o: ${SRC}/rook.hpp ${SRC}/rook.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/rook.cpp
bishop.o: ${SRC}/bishop.hpp ${SRC}/bishop.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/bishop.cpp
knight.o: ${SRC}/knight.hpp ${SRC}/knight.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/knight.cpp
pawn.o: ${SRC}/pawn.hpp ${SRC}/pawn.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/pawn.cpp
piece.o: ${SRC}/piece.hpp ${SRC}/piece.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/piece.cpp
utils.o: ${SRC}/utils.hpp ${SRC}/utils.cpp
	${CXX} ${CXXFLAGS} -c ${SRC}/utils.cpp

clean:
	rm -rf *.o chess
