// g++ -std=c++17 main.cpp -I"include" -L"lib" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o Game

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>

#include "include/SDL.h"
#include "include/SDL_image.h"
//#include "include/SDL_timer.h"

#include "helpers.cpp"
//#include "renderer.cpp"
#include "game.cpp"
#include "levels.cpp"

int WinMain(int argc, char* argv[]) {
	std::cout << "Starting" << std::endl;
	srand(time(NULL));

	bool gameRunning = true;

	Renderer renderer(840, 620);
	renderer.init();


	LevelController* levelMenuController = new LevelController(get_level_menu);
	LevelController* level1Controller = new LevelController(get_level_1);

	LevelController* levelController = levelMenuController;
	levelController->levelOpen(&renderer);

	while(gameRunning) {
		while(!levelController->level->stop) { levelController->level->step(); }
		levelController->levelClose();

		if (levelMenuController != nullptr) {
			levelController = levelMenuController;
			levelController->levelOpen(&renderer);
		}
		else gameRunning = false;
	}


	std::cout << "Exitting" << std::endl;

	levelController->levelClose();
	delete levelController;
	levelController = nullptr;


	levelMenuController->levelClose();
	delete levelMenuController;
	levelMenuController = nullptr;
	
	level1Controller->levelClose();
	delete level1Controller;
	level1Controller = nullptr;


	renderer.close();
	return 0;
}