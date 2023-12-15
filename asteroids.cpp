#include "asteroids.hpp"
#include "game.hpp"
#include "player.hpp"
#include <cstdlib>
#include <ncurses.h>
#include <thread>
#include <semaphore.h>
#include <chrono>

namespace Asteroids {
	std::vector<Pos> asteroids;

	// Gera asteróides a cada 100ms, com um máximo de 20 simultâneos na tela
	// Uma thread ficará responsável somente por essa função
	void spawn_asteroids() {
		while (Game::running) {
			// down() no semáforo que rege o número de asteroides (verifica se pode gerar novo)
			sem_wait(&Game::currAsts);
			// down() no semáforo da região crítica
			sem_wait(&Game::astMutex);

			asteroids.push_back({rand() % Game::maxX, 1});

			// up() no semáforo da região crítica
			sem_post(&Game::astMutex);
			
			// Pausa a thread pra não gerar asteróides muito rápido
			std::chrono::milliseconds duration(100);
			std::this_thread::sleep_for(duration);
		}
	}

	// Desenha os asteroides na tela
	void draw() {
		for (auto &a : asteroids) {
			mvaddch(a.y, a.x, 'O');
		}
	}

	// Verifica se houve colisão entre o jogador e algum asteroide
	bool check_player_collision() {
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

	// Atualiza a posição dos asteroides na tela, removendo-os se passarem da parte inferior
	void update() {
		for (auto it = asteroids.begin(); it != asteroids.end();) {
			it->y++;
			if (it->y > Game::maxY) {
				it = asteroids.erase(it);
				// Caso um asteroide seja destruido, up() no semáforo que regula o número de asteroids
				sem_post(&Game::currAsts);
			} else
				it++;
		}
	}
}
