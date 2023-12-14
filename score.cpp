#include "asteroids.hpp"
#include "game.hpp"
#include "player.hpp"
#include "score.hpp"
#include <ncurses.h>
#include <iostream>

namespace Score {
	int score = 0;
    std::mutex mtxScore;

    void addScore(){
        Score::mtxScore.lock();
        Score::score++;
        Score::mtxScore.unlock();
    }

    void draw(){
		mvprintw(0, 5, "Score: %d", Score::score);
	}
}; // namespace Score