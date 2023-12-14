#include <ncurses.h>
#include <thread>
#include "game.hpp"
#include "player.hpp"
#include "asteroids.hpp"
#include "score.hpp"
#include <unistd.h>

int main() {
    initscr();
    curs_set(0);
    keypad(stdscr, true);
    timeout(-1);

    Game::running = true;
    getmaxyx(stdscr, Game::maxY, Game::maxX);

    Player::init();

    int score = 0;

	mvprintw(20, 20, "Pressione um botao para iniciar o jogo");
	getch();

    std::thread obstacles(Asteroids::spawn_asteroids);
    std::thread actions(Player::input);
    std::thread rendering(Game::gameLoop);

    obstacles.join();
    actions.join();
    rendering.join();

	clear();
    mvprintw(20, 20, "Game over");
    refresh();

    getch();
    endwin();

    return 0;
}