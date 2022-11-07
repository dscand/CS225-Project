#ifndef levels_default_hpp
#define levels_default_hpp

#include "../include/SDL.h"

#include "../game.hpp"
#include "../renderer.hpp"
#include "../levels.hpp"
#include "../menu.hpp"

namespace _level_default {
	const int buttonIndexes[] = {0,2};
	void init(Level* level, Renderer* renderer);
	int end(Level* level);
	int close(Level* level);
	void step(Level* level);
}

#endif