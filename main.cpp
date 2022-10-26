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

	Renderer renderer(840, 620);
	renderer.init();


	LevelController* levelController = new LevelController(get_level_menu); //get_level_1
	levelController->levelOpen(&renderer);

	/* int i = 2;
	while(i > 0) {
		while(!levelController->level->stop) { levelController->level->step(); }
		levelController->levelRestart();
		i--;
	} */
	while(!levelController->level->stop) { levelController->level->step(); }


	std::cout << "Exitting" << std::endl;

	levelController->levelClose();
	delete levelController;
	levelController = nullptr;

	renderer.close();
	return 0;
}