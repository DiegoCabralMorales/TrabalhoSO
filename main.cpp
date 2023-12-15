#include <ncurses.h>
#include <thread>
#include "game.hpp"
#include "player.hpp"
#include "asteroids.hpp"
#include <unistd.h>

int main() {
    Game::init();
    Player::init();

	Game::drawStartScreen();

    std::thread obstacles(Asteroids::spawn_asteroids);
    std::thread actions(Player::input);
    std::thread rendering(Game::gameLoop);

    obstacles.join();
    actions.join();
    rendering.join();

	Game::drawGameOver();

    endwin();

    return 0;
}