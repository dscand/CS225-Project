// g++ -std=c++17 main.cpp -I"include" -L"lib" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o Game

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>

#include "include/SDL.h"
#include "include/SDL_image.h"
//#include "include/SDL_timer.h"

#include "helpers.cpp"
#include "menu.cpp"
//#include "renderer.cpp"
#include "game.cpp"
#include "levels.cpp"

int WinMain(int argc, char* argv[]) {
	std::cout << "Starting" << std::endl;
	srand(time(NULL));

	bool gameRunning = true;

	Renderer renderer(840, 620);
	renderer.init();


	std::function<Level*()> levels[] = { get_level_menu, get_level_1, get_level_2, get_level_3 };
	int levelsLen = 4;

	LevelController* levelController = new LevelController(levels[0]);
	levelController->levelOpen(&renderer);

	while(gameRunning) {
		while(!levelController->level->stop) { levelController->level->step(); }
		int nextLevel = levelController->levelClose();

		std::cout << "NextLevel: " << nextLevel << std::endl;

		if (nextLevel == -1) gameRunning = false;
		else if (nextLevel == -2) levelController->levelRestart(&renderer);
		else {
			levelController->levelClose();
			delete levelController;
			//levelController = nullptr;

			if (nextLevel < levelsLen) levelController = new LevelController(levels[nextLevel]);
			else levelController = new LevelController(levels[0]);
			levelController->levelOpen(&renderer);
		}
	}


	std::cout << "Exitting" << std::endl;


	levelController->levelClose();
	delete levelController;
	levelController = nullptr;


	renderer.close();
	return 0;
}