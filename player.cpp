#include "player.hpp"
#include "asteroids.hpp"
#include "game.hpp"
#include <ncurses.h>
#include <semaphore.h>

namespace Player {
	int x, y;
	std::vector<Pos> bullets;

	// Define a posição inicial da nave no meio da tela na parte inferior
	void init() {
		x = Game::maxX / 2 - 2;
		y = Game::maxY - 4;
	}

	// Acessa região crítica e altera posição da nave
	void moveLeft() {
		sem_wait(&Game::plyMutex);

		if (x > 0)
			x--;

		sem_post(&Game::plyMutex);
	}

	// Acessa região crítica e altera posição da nave
	void moveRight() {
		sem_wait(&Game::plyMutex);

		if (x < Game::maxX - 1)
			x++;

		sem_post(&Game::plyMutex);
	}

	// Desenha a nave na tela
	void draw_spaceship() {
		attron(COLOR_PAIR(2));
		mvprintw(y, x, "  A");
		mvprintw(y + 1, x, " / \\");
		mvprintw(y + 2, x, "[]-[]");
		attroff(COLOR_PAIR(2));
	}

	// Cria uma nova bala, acessando corretamente a região crítica
	void shoot(){
		sem_wait(&Game::plyMutex);
		bullets.push_back({Player::x+2, Player::y});
		sem_post(&Game::plyMutex);
	}

	// Renderiza as balas na tela
	void draw_bullets(){
		for(auto& a : bullets)
			mvaddch(a.y, a.x, '|');
	}

	// Atualiza posição das balas do jogador
	// Remove balas se sairem da tela ou colidirem com um asteroide
	// Também remove asteroide em caso de colisão
	void update_bullets(){
		for(auto it = bullets.begin(); it != bullets.end();){
			it->y--;

			// Bala saiu da tela
			if(it->y < 2){
				it = bullets.erase(it);
				continue;
			}

			bool hit = false;

			// Itera pelos asteroides pra verificar se colide
			for(auto it2 = Asteroids::asteroids.begin(); it2 != Asteroids::asteroids.end();){
				if(it->y-it2->y <= 1 && it->x == it2->x){
					// Se colidiu, remove bala e asteroide do vetor
					it = bullets.erase(it);
					it2 = Asteroids::asteroids.erase(it2);
					hit = true;

					Game::score += 10; // Aumenta pontução do jogador

					// up() no semáforo que controla o número de asteroides na tela
					sem_post(&Game::currAsts);
				}
				// Se não removeu asteroide precisa incrementar contador
				else it2++;
			}
			// Só incrementa it se não removeu bala
			// Se removeu a própria remoção reposiciona o iterador
			if(!hit) it++;
		}
	}

	// Recebe o input do jogador
	// Uma thread ficará responsável somente por essa função
	void input() {
		timeout(1000);

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

		timeout(-1);
	}
}