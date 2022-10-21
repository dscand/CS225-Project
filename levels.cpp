#include <iostream>
#include <functional>

#include "include/SDL.h"

#include "game.cpp"

class Level {
	protected:
		int id;

	private:
		static int nextId;
		std::function<void(Level*)> _init;
		std::function<void(Level*, long double)> _step;
		std::function<void(Level*)> _close;
		
	public:
		Level(std::function<void(Level*)>, std::function<void(Level*)>);
		Level(std::function<void(Level*)>, std::function<void(Level*)>, std::function<void(Level*, long double)>);
		void init() { _init(this); }
		void step(long double deltaT) { _step(this, deltaT); }
		void close() { _close(this); }
		Renderer* renderer;
		Background* background;
		Player* player;
		std::vector<GravityWell_stationary*> gravityWells_stationary;
		std::vector<GravityWell_moving*> gravityWells_moving;
		const int GAME_WIDTH = 2048;
		const int GAME_HEIGHT = 2048;
};
int Level::nextId = 0;
void gameLevelStep(Level*, long double);

Level::Level(std::function<void(Level*)> init, std::function<void(Level*)> close) {
	_init = init;
	_close = close;
	_step = gameLevelStep;

	gravityWells_stationary = {};
	gravityWells_moving = {};

	id = nextId;
	nextId++;
}
Level::Level(std::function<void(Level*)> init, std::function<void(Level*)> close, std::function<void(Level*, long double)> step) {
	_init = init;
	_close = close;
	_step = step;

	id = nextId;
	nextId++;
}

void gameLevelStep(Level* level, long double deltaT) {
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if(currentKeyStates[SDL_SCANCODE_UP]) {
		level->player->boost(1 * deltaT);
	}
	else {
		level->player->nonboost();
	}
	/*if(currentKeyStates[SDL_SCANCODE_DOWN]) {
		level->player->boost(-1 * deltaT);
	}*/
	if(currentKeyStates[SDL_SCANCODE_RIGHT]) {
		level->player->rotate(1 * deltaT);
	}
	if(currentKeyStates[SDL_SCANCODE_LEFT]) {
		level->player->rotate(-1 * deltaT);
	}


	// Physics Update
	for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
		gravityWell->step(level->GAME_WIDTH, level->GAME_HEIGHT, deltaT);
	}
	level->player->playerStep(level->GAME_WIDTH, level->GAME_HEIGHT, deltaT);


	level->renderer->clear();

	int centerX = level->renderer->centerXCal(level->player->getPosX(), (level->renderer->getWindowWidth() / 2) + level->player->getOffsetX(), level->GAME_WIDTH - level->renderer->getWindowWidth());
	int centerY = level->renderer->centerYCal(level->player->getPosY(), (level->renderer->getWindowHeight() / 2) + level->player->getOffsetY(), level->GAME_HEIGHT - level->renderer->getWindowHeight());

	// Render Update
	level->background->render(centerX, centerY);
	for (GravityWell_stationary* gravityWell : level->gravityWells_stationary) {
		gravityWell->render(centerX, centerY);
	}
	for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
		gravityWell->render(centerX, centerY);
	}
	level->player->render(centerX, centerY);

	level->renderer->update();
}

#include "levels/level_menu.cpp"
Level level_menu(_level_menu::init, _level_menu::close, _level_menu::step);

#include "levels/level_1.cpp"
Level level_1(_level_1::init, _level_1::close);