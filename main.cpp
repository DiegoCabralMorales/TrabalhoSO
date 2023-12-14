#include <iostream>
#include <ncurses.h>
#include <semaphore.h>
#include <thread>
#include <unistd.h>
#include <vector>

struct Pos {
	int x, y;
};

namespace Game {
	int maxY, maxX;

	bool running = true;
} // namespace Game

namespace Player {
	int x, y;

	std::vector<Pos> bullets;

	void moveLeft() {
		if (x > 0)
			x--;
	}

	void moveRight() {
		if (x < Game::maxX - 1)
			x++;
	}

	void draw() {
		mvprintw(y, x, "  A");
		mvprintw(y + 1, x, " / \\");
		mvprintw(y + 2, x, "[]-[]");
	}

	void shoot() {}
}; // namespace Player

namespace Asteroids {
	std::vector<Pos> asteroids;
	int max = 20;

	void spawn_asteroids() {
		while (Game::running) {
			if (asteroids.size() < max)
				asteroids.push_back(Pos{rand() % Game::maxX, 0});

			usleep(100 * 1000);
		}
	}

	void draw() {
		for (auto &a : asteroids) {
			mvaddch(a.y, a.x, 'O');
		}
	}

	int check_colisions() {
		for (auto &a : asteroids) {
			if (a.y == Player::y && a.x == Player::x + 2)
				return Game::running = 0;
			if (a.y == Player::y + 1 && a.x > Player::x && a.x < Player::x + 3)
				return Game::running = 0;
			if (a.y == Player::y + 2 && a.x >= Player::x && a.x <= Player::x + 3)
				return Game::running = 0;
		}

		return 1;
	}

	void update() {
		for (auto it = asteroids.begin(); it != asteroids.end(); it++) {
			it->y++;
			if (it->y > Game::maxY)
				asteroids.erase(it);
		}

		check_colisions();
	}
} // namespace Asteroids

int main() {
	initscr();
	curs_set(0);
	keypad(stdscr, true);
	timeout(0);

	getmaxyx(stdscr, Game::maxY, Game::maxX);

	Player::x = Game::maxX / 2 - 2;
	Player::y = Game::maxY - 3;

	int score = 0;

	std::thread obstacles(Asteroids::spawn_asteroids);

	// Game loop
	while (Game::running) {
		switch (getch()) {
		case 'q':
			Game::running = 0;
			break;
		case KEY_LEFT:
			Player::moveLeft();
			break;
		case KEY_RIGHT:
			Player::moveRight();
			break;
		case 'x':
			Player::shoot();
			break;
		}

		// Render
		clear();
		Player::draw();
		Asteroids::draw();
		refresh();
		Asteroids::update();

		// Pause to slow down the game
		usleep(30 * 1000);
	}
	
	clear();
	mvprintw(20, 20, "Game over");

	timeout(-1);
	getch();

	endwin();

	obstacles.join();

	return 0;
}
