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

const int WINDOW_WIDTH = 840;
const int WINDOW_HEIGHT = 840;

int WinMain(int argc, char* argv[]) {
	std::cout << "Starting" << std::endl;
	srand(time(NULL));

	Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);


	std::cout << "Level 1, Start" << std::endl;
	Level* level = get_level_1();
	level->init(&renderer);

	while(!level->levelController->stop) { level->step(); }

	level->close();
	delete level;
	std::cout << "Level 1, End" << std::endl;


	std::cout << "Exitting" << std::endl;
	return 0;
}