#ifndef GAME_H
#define GAME_H

#include <mutex>

namespace Game {
    extern int maxY, maxX;
    extern int score;
    extern bool running;

    extern std::mutex astMutex;
    extern std::mutex plyMutex;

    void init();
    void drawStartScreen();
    void gameLoop();
    void drawScore();
    void drawGameOver();
}

#endif // GAME_H