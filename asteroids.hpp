#ifndef ASTEROIDS_H
#define ASTEROIDS_H

struct Pos;

#include <vector>

namespace Asteroids {
	extern std::vector<Pos> asteroids;

	void spawn_asteroids();
	void draw();
	bool check_player_collision();
	void update();
}

#endif // ASTEROIDS_H