#include <ncurses.h>
#include <thread>
#include "game.hpp"
#include "player.hpp"
#include "asteroids.hpp"
#include <unistd.h>

int main() {
	// Inicializa o ambiente do jogo
	Game::init();
	Player::init();

	Game::StartScreen();

	// Inicia as threads para a geração de obstáculos (asteróides), entrada do jogador e renderização do jogo
	std::thread obstaculos(Asteroids::spawn_asteroids);
	std::thread acoes(Player::input);
	std::thread renderizacao(Game::gameLoop);

	// Aguarda o término das threads
	obstaculos.join();
	acoes.join();
	renderizacao.join();

	Game::GameOver();
	
	return 0;
}
