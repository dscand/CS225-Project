#ifndef level_1_hpp
#define level_1_hpp

#include <vector>

#include "../game.hpp"
#include "../renderer.hpp"
#include "../levels.hpp"
#include "../helpers.hpp"

#include "levels_default.hpp"

namespace _level_1 {
	void init(Level* level, Renderer* renderer);
	int end(Level* level);
	void step(Level* level);
}

#endif