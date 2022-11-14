#include "levels.hpp"

int Level::loadedLevels = 0;

Level::Level(int levelID, int scoreGoal, int nextLevel, std::function<void(Level*, Renderer*)> init, std::function<int(Level*)> end, std::function<void(Level*)> step, std::function<int(Level*)> close) {
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
	this->levelID = levelID;
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
void Level::end() {
	completionTime = _end(this); ended = true;
	endTimer.start();
	std::cout << (double)completionTime / 1000. << " : " << score << std::endl;
	writeHighscore(levelID, score, scoreGoal, completionTime);
}


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