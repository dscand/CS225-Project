#ifndef levels_hpp
#define levels_hpp

#include <iostream>
#include <functional>

#include "include/SDL.h"

#include "game.hpp"
#include "highscore.hpp"


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
		Level(int, int, int, std::function<void(Level*, Renderer*)>, std::function<int(Level*)>, std::function<void(Level*)>, std::function<int(Level*)>);
		~Level() { close(); loadedLevels--; }
		void init(Renderer* renderer) { _init(this, renderer); }
		void step() { _step(this); }
		void end();
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
		int levelID;
		LTimer dtTimer;
		float timeSpeed;

		int completionTime;
		int nextLevel;
};

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

#endif