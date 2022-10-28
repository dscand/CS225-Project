#include <iostream>
#include <functional>

#include "include/SDL.h"

#include "game.cpp"

class Level {
	protected:
		static int loadedLevels;

	private:
		std::function<void(Level*, Renderer*)> _init;
		std::function<int(Level*)> _end;
		std::function<void(Level*)> _close;
		std::function<void(Level*)> _step;

		int gameWidth;
		int gameHeight;
		
	public:
		Level(int, std::function<void(Level*, Renderer*)>, std::function<int(Level*)>, std::function<void(Level*)>, std::function<void(Level*)>);
		~Level() { close(); loadedLevels--; }
		void init(Renderer* renderer) { _init(this, renderer); }
		void step() { _step(this); }
		void end() { completionTime = _end(this); ended = true; endTimer.start();; std::cout << (double)completionTime / 1000. << " : " << score << std::endl; }
		void close() { return _close(this); }
		int getGameWidth() { return gameWidth; }
		int getGameHeight() { return gameHeight; }
		void setGameWidth(int width) { gameWidth = width; }
		void setGameHeight(int height) { gameHeight = height; }

		Renderer* renderer;

		Background* background;
		std::vector<Texture*> textures;
		Player* player;
		std::vector<GravityWell_stationary*> gravityWells_stationary;
		std::vector<GravityWell_moving*> gravityWells_moving;
		std::vector<StarCoin*> starCoins;

		LTimer gameTime;
		LTimer endTimer;

		bool alive;
		bool stop;
		bool ended;
		bool pause;
		int score;
		int scoreGoal;
		LTimer dtTimer;
		float timeSpeed;

		int completionTime;
};
int Level::loadedLevels = 0;
void gameLevelStep(Level*);

Level::Level(int scoreGoal, std::function<void(Level*, Renderer*)> init, std::function<int(Level*)> end, std::function<void(Level*)> close, std::function<void(Level*)> step = gameLevelStep) {
	_init = init;
	_end = end;
	_close = close;
	_step = step;

	this->gameWidth = 2048;
	this->gameHeight = 2048;

	alive = true;
	stop = false;
	pause = false;
	score = 0;
	this->scoreGoal = scoreGoal;
	timeSpeed = 1;
	completionTime = 0;

	gameTime.start();

	textures = {};
	gravityWells_stationary = {};
	gravityWells_moving = {};

	loadedLevels++;
}

void gameLevelStep(Level* level) {
	float deltaT = level->dtTimer.getTicks() / 1000.f;
	if(level->timeSpeed != 1) {
		deltaT /= level->timeSpeed;
	}
	level->dtTimer.start();

	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			// close/X button
			case SDL_QUIT:
				level->stop = true;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_CLOSE:
						level->stop = true;
						break;
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
						level->pause = true;
						break;
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						level->renderer->setWindow(event.window.data1, event.window.data2);
						break;

					default:
						break;
				}

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

	if (level->pause) {
		// TODO: Pause Menu
	}
	else {
		// Control Step
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


		// Physics Step
		for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
			gravityWell->step(deltaT);
		}
		level->player->playerStep(level->getGameWidth(), level->getGameHeight(), deltaT);

		const int endTime = 4000; // ms
		if (!level->player->isAlive()) {
			if (!level->endTimer.isStarted()) {
				level->player->explosionIndex = 1;
				level->end();
			} 
			else {
				const int explosionTime = 500; // ms
				int ticks = level->endTimer.getTicks();
				if (ticks > endTime) {
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
		else if (level->ended) {
			int ticks = level->endTimer.getTicks();
			if (ticks > endTime) {
				level->stop = true;
			}
		}


		// Game Step
		const int coinRange = 20;
		for (StarCoin* coin : level->starCoins) {
			long double dist = calcDistance(level->player->getPosX(), level->player->getPosY(), coin->getPosX(), coin->getPosY());
			if (level->player->isAlive() && dist <= coinRange && coin->active) {
				coin->active = false;
				level->score++;

				if (level->scoreGoal > 0 && level->score >= level->scoreGoal) {
					level->end();
					//level->stop = true;
				}
			}
		}


		// Rendering Step
		level->renderer->clear();

		int centerX = level->renderer->centerXCal(level->player->getPosX(), (level->renderer->getWindowWidth() / 2) + level->player->getOffsetX(), level->getGameWidth() - level->renderer->getWindowWidth());
		int centerY = level->renderer->centerYCal(level->player->getPosY(), (level->renderer->getWindowHeight() / 2) + level->player->getOffsetY(), level->getGameHeight() - level->renderer->getWindowHeight());

		// Render Update
		level->background->render(centerX, centerY);
		for (GravityWell_stationary* gravityWell : level->gravityWells_stationary) {
			gravityWell->render(centerX, centerY);
		}
		for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
			gravityWell->render(centerX, centerY);
		}
		for (StarCoin* starCoin : level->starCoins) {
			starCoin->render(centerX, centerY);
		}
		for (Texture* texture : level->textures) {
			texture->render(centerX, centerY);
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
		void levelDelete();
};
LevelController::LevelController(std::function<Level*()> levelFunction) {
	level = NULL;
	this->levelFunction = levelFunction;
}
void LevelController::levelOpen(Renderer* renderer) {
	std::cout << "Level, Init" << std::endl;
	if (level != nullptr) levelClose();
	level = this->levelFunction();
	level->init(renderer);
}
void LevelController::levelClose() {
	if (level != nullptr) {
		std::cout << "Level, Close" << std::endl;
		delete level;
		level = nullptr;
	}
}
void LevelController::levelDelete() {
	levelClose();
	delete this;
}
void LevelController::levelRestart() {
	std::cout << "Level, Restart" << std::endl;
	Renderer* renderer = level->renderer;
	levelClose();
	levelOpen(renderer);
}


#include "levels/level_menu.cpp"
Level* get_level_menu() {
	return new Level(0, _level_menu::init, _level_menu::end, _level_menu::close, _level_menu::step);
}

#include "levels/level_1.cpp"
Level* get_level_1() {
	return new Level(1, _level_1::init, _level_1::end, _level_1::close);
}