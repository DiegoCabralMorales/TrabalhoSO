#ifndef GAME_H
#define GAME_H

#include <semaphore.h>

namespace Game {
	extern int maxY, maxX;
	extern int score;
	extern bool running;

	extern sem_t currAsts;
	extern sem_t astMutex;
	extern sem_t plyMutex;

	void init();
	void StartScreen();
	void gameLoop();
	void drawScore();
	void GameOver();
}

#endif // GAME_H