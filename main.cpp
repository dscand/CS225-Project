#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>

#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_mixer.h"
#include "include/SDL_timer.h"

#include "helpers.hpp"
#include "menu.hpp"
#include "renderer.hpp"
#include "game.hpp"
#include "levels.hpp"


#include "levels/levels_default.hpp"
#include "levels/level_menu.hpp"
#include "levels/level_1.hpp"
#include "levels/level_2.hpp"
#include "levels/level_3.hpp"

Level* get_level_menu() {
	return new Level(-1, 0, -1, _level_menu::init, _level_menu::end, _level_menu::step, _level_default::close);
}

Level* get_level_1() {
	return new Level(1, 4, 2, _level_1::init, _level_default::end, _level_default::step, _level_default::close);
}
Level* get_level_2() {
	return new Level(2, 1, 3, _level_2::init, _level_default::end, _level_default::step, _level_default::close);
}
Level* get_level_3() {
	return new Level(3, 4, 4, _level_3::init, _level_default::end, _level_default::step, _level_default::close);
}


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