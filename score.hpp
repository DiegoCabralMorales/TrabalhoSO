#ifndef SCORE_H
#define SCORE_H

#include <mutex>

namespace Score {
    extern int Score;
    extern std::mutex mtxScore;

    void addScore();
    void draw();
}

#endif // SCORE_H
