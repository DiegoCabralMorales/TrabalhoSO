#include "asteroids.hpp"
#include "game.hpp"
#include "player.hpp"
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>

namespace Asteroids {
	std::vector<Pos> asteroids;
	int max = 20;

	void spawn_asteroids() {
		while (Game::running) {
			Game::astMutex.lock();

			if (asteroids.size() < max)
				asteroids.push_back({rand() % Game::maxX, 1});

			Game::astMutex.unlock();

			usleep(100 * 1000);
		}
	}

	void draw() {
		for (auto &a : asteroids) {
			mvaddch(a.y, a.x, 'O');
		}
	}

	bool check_player_colision() {
		for (auto &a : asteroids) {
			if (a.y == Player::y && a.x == Player::x + 2)
				return true;
			if (a.y == Player::y + 1 && a.x > Player::x && a.x < Player::x + 4)
				return true;
			if (a.y == Player::y + 2 && a.x >= Player::x && a.x <= Player::x + 4)
				return true;
		}

		return false;
	}

	void update() {
		for (auto it = asteroids.begin(); it != asteroids.end();) {
			it->y++;
			if (it->y > Game::maxY)
				it = asteroids.erase(it);
			else
				it++;
		}
	}
} // namespace Asteroids