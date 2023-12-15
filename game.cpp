#include "player.hpp"
#include "asteroids.hpp"
#include "game.hpp"
#include <chrono>
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <thread>
#include <ctime>
#include <semaphore.h>

namespace Game {
	int maxY, maxX;
	bool running = true;
	int score = 0;

	std::string separator;
	sem_t currAsts;
	sem_t astMutex;
	sem_t plyMutex;
	
	// Inicializa a janela da biblioteca ncurses, define algumas opções/constantes, inicializa os semáforos
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
		init_pair(2, COLOR_BLUE, COLOR_BLACK);

		separator = std::string(maxX, '-');
		
		sem_init(&currAsts, 0, 20); // 20 é o máximo de asteroides na tela por vez
		sem_init(&astMutex, 0, 1); // Mutex
		sem_init(&plyMutex, 0, 1); // Mutex
	}

	// Gera a tela inicial do jogo
	void StartScreen(){
		int yStart = Game::maxY/3;
		int xStart = Game::maxX/2 - 22;

		// Posiciona alguns asteroides aleatoriamente
		for(int i = 0; i < 50; i++){
			int x = rand() % Game::maxX;
			int y = rand() % Game::maxY;
			mvaddch(y, x, 'O');
		}
		
		// Desenha a nave do jogador e o nome do jogo
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

	void GameOver(){
		clear();

		int yStart = Game::maxY/3;
		int xStart = Game::maxX/2 - 28;

		// Desenha a tela de Game Over
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
		
		// Espera o usuário apertar ESC para fechar
		int c, ESC = 27;
		while((c = getch()) != ESC);

		// Finaliza a janela do ncurses
		endwin();

		// Destroi (free) nos semáforos
		sem_destroy(&Game::currAsts);
		sem_destroy(&Game::astMutex);
		sem_destroy(&Game::plyMutex);
	}

	// Utilitário pra exibir a pontuação do score e um separador entre o jogo e a HUD
	void drawHUD(){
		std::string scoreStr = "Score: " + std::to_string(Game::score);
		int xpos = Game::maxX/2 - scoreStr.size()/2;
		
		mvprintw(0, xpos, "Score: %d", Game::score);
		mvprintw(1, 0, "%s", separator.c_str());
	}

	// Atualiza e desenha os elementos do jogo a cada 50ms
	// Uma thread ficará responsável por essa função
	void gameLoop(){
		while (Game::running){
			// Precisa fazer leitura/escrita na região crítica para as atualizações e renders
			// down() nos semáforos para garantir que não está sendo gerado nenhum asteroide nem o player executando ação
			sem_wait(&plyMutex);
			sem_wait(&astMutex);

			clear(); // Limpa a tela

			// Atualizações
			Player::update_bullets();
			Asteroids::update();

			// Renders
			Player::draw_spaceship();
			Player::draw_bullets();
			Asteroids::draw();

			// Checa se o jogo deve continuar rodando
			Game::running = !Asteroids::check_player_collision();

			// up() nos semáforos de região crítica
			sem_post(&astMutex);
			sem_post(&plyMutex);

			drawHUD(); // Desenha o score
			refresh();

			// Espera 50ms pra que o jogo não seja rápido demais
			std::chrono::milliseconds duration(50);
			std::this_thread::sleep_for(duration);
		}
	}
}