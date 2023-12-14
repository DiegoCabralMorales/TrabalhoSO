#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <semaphore.h>

struct Asteroid {
    bool v;
    int x, y;

    Asteroid(int x, int y) : x(x), y(y), v(true) {}
};

namespace Asteroids { 
    std::vector<Asteroid> pos;

    bool check_colisions();

    // Asteroid asteroid(rand() % maxX, 0);
}

namespace Game {
    int maxY, maxX;
}

struct Player {
    int x, y;

    Player() : x(Game::maxX/2 - 2), y(Game::maxY - 3) {}

    void moveLeft() {
        if (x > 0) x--;
    }

    void moveRight() {
        if (x < Game::maxX - 1) x++;
    }

    void draw(){
        mvprintw(y, x, "  A");
        mvprintw(y+1, x, " / \\");
        mvprintw(y+2, x, "[]-[]");
    }
};


int main() {
    initscr();
    curs_set(0);
    keypad(stdscr, true);
    timeout(0);
    
    getmaxyx(stdscr, Game::maxY, Game::maxX);

    // Create player and initial asteroid
    Player player;
    
    int score = 0;

    // Game loop
    while (true) {
        switch (getch()) {
            case 'q':
                endwin(); // Clean up and exit
                return 0;
            case KEY_LEFT:
                player.moveLeft();
                break;
            case KEY_RIGHT:
                player.moveRight();
                break;
        }


        // Generate a new asteroid when the previous one reaches the bottom
        // if (asteroid.getY() == maxY - 1) {
        //     asteroid = Asteroid(rand() % maxX, 0);
        //     score++;
        // }

        // Render
        clear();
        player.draw();
        refresh();

        // Pause to slow down the game
        // usleep(100 * 1000); // 10ms delay
    }

    // Clean up
    endwin();

    return 0;
}
