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
		std::function<int(Level*)> _close;
		std::function<void(Level*)> _step;

		int gameWidth;
		int gameHeight;
		
	public:
		Level(int, int, std::function<void(Level*, Renderer*)>, std::function<int(Level*)>, std::function<void(Level*)>, std::function<int(Level*)>);
		~Level() { close(); loadedLevels--; }
		void init(Renderer* renderer) { _init(this, renderer); }
		void step() { _step(this); }
		void end() { completionTime = _end(this); ended = true; endTimer.start();; std::cout << (double)completionTime / 1000. << " : " << score << std::endl; }
		int close() { return _close(this); }
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
		int nextLevel;
};
int Level::loadedLevels = 0;

#include "levels/levels_default.cpp"
Level::Level(int scoreGoal, int nextLevel, std::function<void(Level*, Renderer*)> init, std::function<int(Level*)> end = _level_default::end, std::function<void(Level*)> step = _level_default::step, std::function<int(Level*)> close = _level_default::close) {
	_init = init;
	_end = end;
	_close = close;
	_step = step;

	this->gameWidth = 2048;
	this->gameHeight = 2048;

	this->nextLevel = nextLevel;

	alive = true;
	stop = false;
	pause = false;
	score = 0;
	this->scoreGoal = scoreGoal;
	timeSpeed = 1;
	completionTime = 0;

	gameTime.start();

	background = nullptr;
	textures = {};
	player = nullptr;
	gravityWells_stationary = {};
	gravityWells_moving = {};
	starCoins = {};

	loadedLevels++;
}


class LevelController {
	private:
		std::function<Level*()> levelFunction;

	public:
		LevelController(std::function<Level*()>);
		~LevelController() { levelClose(); };

		Level* level;
		void levelOpen(Renderer* renderer);
		void levelRestart(Renderer* renderer);
		int levelEnd();
		int levelClose();
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
	std::cout << level->scoreGoal << " stars" << std::endl;
}
int LevelController::levelClose() {
	if (level != nullptr) {
		std::cout << "Level, Close" << std::endl;

		int nextLevel = level->close();
		delete level;
		level = nullptr;

		return nextLevel;
	}
	else return 0;
}
void LevelController::levelRestart(Renderer* renderer) {
	std::cout << "Level, Restart" << std::endl;
	levelClose();
	levelOpen(renderer);
}


#include "levels/level_menu.cpp"
Level* get_level_menu() {
	return new Level(0, -1, _level_menu::init, _level_menu::end, _level_menu::step);
}

#include "levels/level_1.cpp"
Level* get_level_1() {
	return new Level(4, 2, _level_1::init);
}

#include "levels/level_2.cpp"
Level* get_level_2() {
	return new Level(1, 3, _level_2::init);
}

#include "levels/level_3.cpp"
Level* get_level_3() {
	return new Level(5, 4, _level_3::init);
}