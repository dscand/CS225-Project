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
		std::function<int(Level*)> _end;
		std::function<void(Level*)> _close;
		std::function<void(Level*)> _step;
		
	public:
		Level(std::function<void(Level*, Renderer*)>, std::function<int(Level*)>, std::function<void(Level*)>, std::function<void(Level*)>);
		~Level() { close(); }
		void init(Renderer* renderer) { _init(this, renderer); }
		void step() { _step(this); }
		int end() { return _end(this); }
		void close() { return _close(this); }
		Renderer* renderer;
		Background* background;
		Player* player;
		std::vector<GravityWell_stationary*> gravityWells_stationary;
		std::vector<GravityWell_moving*> gravityWells_moving;

		const int GAME_WIDTH = 2048;
		const int GAME_HEIGHT = 2048;

		LTimer gameTime;
		LTimer deathTime;

		bool alive;
		bool stop;
		bool pause;
		LTimer dtTimer;
		float timeSpeed;

		int completionTime;
};
int Level::nextId = 0;
void gameLevelStep(Level*);

Level::Level(std::function<void(Level*, Renderer*)> init, std::function<int(Level*)> end, std::function<void(Level*)> close, std::function<void(Level*)> step = gameLevelStep) {
	_init = init;
	_end = end;
	_close = close;
	_step = step;

	alive = true;
	stop = false;
	pause = false;
	timeSpeed = 1;
	completionTime = 0;

	gameTime.start();

	gravityWells_stationary = {};
	gravityWells_moving = {};

	id = nextId;
	nextId++;
}

void gameLevelStep(Level* level) {
	float deltaT = level->dtTimer.getTicks() / 1000.f;
	if(level->timeSpeed != 1) {
		deltaT /= level->timeSpeed;
	}
	level->dtTimer.start();

	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				// handling of close button
				level->stop = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						level->pause = !level->pause;
						break;
					case SDL_SCANCODE_1:
					case SDL_SCANCODE_KP_1:
						level->timeSpeed = 1;
						break;
					case SDL_SCANCODE_2:
					case SDL_SCANCODE_KP_2:
						level->timeSpeed = 2;
						break;
					case SDL_SCANCODE_3:
					case SDL_SCANCODE_KP_3:
						level->timeSpeed = 4;
						break;
					case SDL_SCANCODE_4:
					case SDL_SCANCODE_KP_4:
						level->timeSpeed = 8;
						break;

					default:
						break;
				}
				
			default:
				break;
		}
	}

	if(level->pause) {
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
			if (!level->deathTime.isStarted()) {
				level->deathTime.start();
				level->player->explosionIndex = 1;
				level->completionTime = level->end();
			} 
			else {
				const int explosionTime = 500; // ms
				const int deathTime = 4000; // ms
				int ticks = level->deathTime.getTicks();
				if (ticks > deathTime) {
					level->stop = true;
				}
				else if (ticks > 3 * explosionTime) {
					level->player->explosionIndex = 0;
				}
				else if (ticks > 2 * explosionTime) {
					level->player->explosionIndex = 3;
				}
				else if (ticks > explosionTime) {
					level->player->explosionIndex = 2;
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


class LevelController {
	private:
		std::function<Level*()> levelFunction;

	public:
		LevelController(std::function<Level*()>);
		~LevelController() { levelClose(); };

		Level* level;
		void levelOpen(Renderer* renderer);
		void levelRestart();
		int levelEnd();
		void levelClose();
};
LevelController::LevelController(std::function<Level*()> levelFunction) {
	level = NULL;
	this->levelFunction = levelFunction;
}
void LevelController::levelOpen(Renderer* renderer) {
	levelClose();
	level = this->levelFunction();
	level->init(renderer);
}
void LevelController::levelClose() {
	delete level;
	level = nullptr;
}
void LevelController::levelRestart() {
	Renderer* renderer = level->renderer;
	levelClose();
	levelOpen(renderer);
}


#include "levels/level_menu.cpp"
Level* get_level_menu() {
	return new Level(_level_menu::init, _level_menu::end, _level_menu::close, _level_menu::step);
}

#include "levels/level_1.cpp"
Level* get_level_1() {
	return new Level(_level_1::init, _level_1::end, _level_1::close);
}