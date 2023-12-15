#include "player.hpp"
#include "asteroids.hpp"
#include "game.hpp"
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <ctime>

namespace Game {
  int maxY, maxX;
  bool running = true;
  int score = 0;

  std::string separator;

  std::mutex astMutex;
  std::mutex plyMutex;

  void init(){
    srand(time(0));

    initscr();
    curs_set(0);
    keypad(stdscr, true);
    timeout(-1);
    start_color();
    noecho();

    getmaxyx(stdscr, Game::maxY, Game::maxX);

    init_pair(1, COLOR_RED, COLOR_BLACK);

    separator = std::string(maxX, '-');
  }

  void drawStartScreen(){
    int yStart = Game::maxY/3;
    int xStart = Game::maxX/2 - 22;

    for(int i = 0; i < 50; i++){
      int x = rand() % Game::maxX;
      int y = rand() % Game::maxY;
      mvaddch(y, x, 'O');
    }

    Player::draw_spaceship();

    mvprintw(yStart, xStart, "              _                 _     _     ");
    mvprintw(yStart+1, xStart, "    /\\       | |               (_)   | |    ");
    mvprintw(yStart+2, xStart, "   /  \\   ___| |_ ___ _ __ ___  _  __| |___ ");
    mvprintw(yStart+3, xStart, "  / /\\ \\ / __| __/ _ \\ '__/ _ \\| |/ _` / __|");
    mvprintw(yStart+4, xStart, " / ____ \\\\__ \\ ||  __/ | | (_) | | (_| \\__ \\");
    mvprintw(yStart+5, xStart, "/_/    \\_\\___/\\__\\___|_|  \\___/|_|\\__,_|___/");

    mvprintw(yStart+7, xStart+3, "Pressione qualquer tecla para começar!");

    refresh();

    getch();
  }

  void drawGameOver(){
    clear();

    int yStart = Game::maxY/3;
    int xStart = Game::maxX/2 - 28;

    attron(COLOR_PAIR(1));
    mvprintw(yStart, xStart, " _____   ___  ___  ___ _____   _____  _   _ ___________ ");
    mvprintw(yStart+1, xStart, "|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | |  ___| ___ \\");
    mvprintw(yStart+2, xStart, "| |  \\// /_\\ \\| .  . || |__   | | | || | | | |__ | |_/ /");
    mvprintw(yStart+3, xStart, "| | __ |  _  || |\\/| ||  __|  | | | || | | |  __||    / ");
    mvprintw(yStart+4, xStart, "| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ / |___| |\\ \\ ");
    mvprintw(yStart+5, xStart, " \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/\\____/\\_| \\_|");
    attroff(COLOR_PAIR(1));

    std::string scoreStr = "Score: " + std::to_string(Game::score);
    int offset = 28 - scoreStr.size()/2;
    mvprintw(yStart+7, xStart+offset, "%s", scoreStr.c_str());
    mvprintw(yStart+9, xStart+13, "Pressione ESC para sair do jogo");
    
    int c, ESC = 27;
    while((c = getch()) != ESC);
  }

  void drawHUD(){
    std::string scoreStr = "Score: " + std::to_string(Game::score);
    int xpos = Game::maxX/2 - scoreStr.size()/2;
    
    mvprintw(0, xpos, "Score: %d", Game::score);
    mvprintw(1, 0, "%s", separator.c_str());
  }

  void gameLoop(){
      int frameMod = 0;

      while (Game::running){
      // Render
        astMutex.lock();
        plyMutex.lock();

        clear();

        Player::update_bullets();
        Asteroids::update();

        Player::draw_spaceship();
        Player::draw_bullets();
        Asteroids::draw();

        Game::running = !Asteroids::check_player_colision();

        plyMutex.unlock();
        astMutex.unlock();

        frameMod = (frameMod+1)%100;
        if(frameMod == 99)
          Game::score += 20;

        drawHUD();
        refresh();

        // Pause to slow down the game
        usleep(50 * 1000);
    }
  }
}