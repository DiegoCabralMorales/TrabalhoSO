all:
	g++ *.cpp -o asteroidsgame.out -std=c++11 -lpthread -lncurses

run:
	./asteroidsgame.out