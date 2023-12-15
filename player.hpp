#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

struct Pos {
	int x, y;
};

namespace Player {
	extern int x, y;
	extern std::vector<Pos> bullets;

	void init();
	void draw_spaceship();
	void draw_bullets();
	void update_bullets();
	void input();
}

#endif // PLAYER_H
