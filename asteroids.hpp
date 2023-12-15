#ifndef ASTEROIDS_H
#define ASTEROIDS_H

struct Pos;

#include <vector>

namespace Asteroids {
    extern std::vector<Pos> asteroids;
    extern int max;

    void init();
    void spawn_asteroids();
    void draw();
    bool check_player_colision();
    void update();
}

#endif // ASTEROIDS_H