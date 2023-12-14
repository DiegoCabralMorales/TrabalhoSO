#include "player.hpp"
#include "asteroids.hpp"
#include "game.hpp"
#include "score.hpp"
#include <ncurses.h>

namespace Player {
	int x, y;
	std::vector<Pos> bullets;

  void init() {
    x = Game::maxX / 2 - 2;
    y = Game::maxY - 3;
  }

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

	void shoot(){
		bullets.push_back(Pos{Player::x+2, Player::y-1});
	}

	void draw_bullets(){
		for(auto& a : bullets)
			mvaddch(a.y, a.x, '|');
	}

	void update_bullets(){
		for(auto it = bullets.begin(); it != bullets.end();){
			it->y--;

			if(it->y < 0){
				it = bullets.erase(it);
				continue;
			}

			bool hit = false;
			
			for(auto it2 = Asteroids::asteroids.begin(); it2 != Asteroids::asteroids.end();){
				if(it->y == it2->y && it->x == it2->x){
					it = bullets.erase(it);
					it2 = Asteroids::asteroids.erase(it2);
					hit = true;
					Score::addScore();
					break;
				}

				else
					it2++;
			}

			if(!hit) it++;
		}
	}

	void input() {
		while (Game::running) {
			int c = getch();

			switch (c) {
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
		}
	}
}; // namespace Player