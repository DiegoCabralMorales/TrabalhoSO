#include "player.hpp"
#include "asteroids.hpp"
#include "game.hpp"
#include "score.hpp"
#include <ncurses.h>
#include <unistd.h>

namespace Game {
  int maxY, maxX;
  bool running;

  void gameLoop(){
      while (Game::running){
      // Render
        Player::update_bullets();
        Asteroids::update();
        clear();
        Player::draw();
        Player::draw_bullets();
        Asteroids::draw();
        Score::draw();
        refresh();

        // Pause to slow down the game
        usleep(50 * 1000);
    }
  }
}