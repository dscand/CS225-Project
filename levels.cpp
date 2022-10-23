#include <iostream>
#include <functional>

#include "include/SDL.h"

#include "game.cpp"

class Level {
	protected:
		int id;

	private:
		static int nextId;
		std::function<void(Level*, Renderer*)> _init;
		std::function<int(Level*)> _close;
		std::function<void(Level*)> _step;
		
	public:
		Level(std::function<void(Level*, Renderer*)>, std::function<int(Level*)>);
		Level(std::function<void(Level*, Renderer*)>, std::function<int(Level*)>, std::function<void(Level*)>);
		void init(Renderer* renderer) { _init(this, renderer); }
		void step() { _step(this); }
		int close() { return _close(this); }
		LevelController* levelController;
		Renderer* renderer;
		Background* background;
		Player* player;
		std::vector<GravityWell_stationary*> gravityWells_stationary;
		std::vector<GravityWell_moving*> gravityWells_moving;
		const int GAME_WIDTH = 2048;
		const int GAME_HEIGHT = 2048;
		bool alive;
};
int Level::nextId = 0;
void gameLevelStep(Level*);

Level::Level(std::function<void(Level*, Renderer*)> init, std::function<int(Level*)> close) {
	_init = init;
	_close = close;
	_step = gameLevelStep;

	gravityWells_stationary = {};
	gravityWells_moving = {};

	id = nextId;
	nextId++;
}
Level::Level(std::function<void(Level*, Renderer*)> init, std::function<int(Level*)> close, std::function<void(Level*)> step) {
	_init = init;
	_close = close;
	_step = step;

	id = nextId;
	nextId++;
}

void gameLevelStep(Level* level) {
	float deltaT = level->levelController->dtTimer.getTicks() / 1000.f;
	if (level->levelController->timeSpeed != 1) {
		deltaT /= level->levelController->timeSpeed;
	}
	level->levelController->dtTimer.start();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				// handling of close button
				level->levelController->stop = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						level->levelController->pause = !level->levelController->pause;
						break;
					case SDL_SCANCODE_1:
					case SDL_SCANCODE_KP_1:
						level->levelController->timeSpeed = 1;
						break;
					case SDL_SCANCODE_2:
					case SDL_SCANCODE_KP_2:
						level->levelController->timeSpeed = 2;
						break;
					case SDL_SCANCODE_3:
					case SDL_SCANCODE_KP_3:
						level->levelController->timeSpeed = 4;
						break;
					case SDL_SCANCODE_4:
					case SDL_SCANCODE_KP_4:
						level->levelController->timeSpeed = 8;
						break;

					default:
						break;
				}
				
			default:
				break;
		}
	}

	if(level->levelController->pause) {
		// TODO: Pause Menu
	}
	else {
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

		if (!level->player->isAlive()) {
			if (!level->levelController->deathTime.isStarted()) {
				level->levelController->deathTime.start();
			} 
			else {
				if (level->levelController->deathTime.getTicks() > 4000) {
					level->levelController->stop = true;
				}
			}
		}


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

	//std::cout << deltaT << std::endl;
	// calculates to 60 fps
	SDL_Delay(1000 / 60);
}

#include "levels/level_menu.cpp"
Level* get_level_menu() {
	return new Level(_level_menu::init, _level_menu::close, _level_menu::step);
}

#include "levels/level_1.cpp"
Level* get_level_1() {
	return new Level(_level_1::init, _level_1::close);
}