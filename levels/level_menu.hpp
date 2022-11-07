#ifndef level_menu_hpp
#define level_menu_hpp

#include <string>

#include "../game.hpp"
#include "../renderer.hpp"
#include "../levels.hpp"
#include "../helpers.hpp"
#include "../menu.hpp"

namespace _level_menu {
	const int buttonIndexes[] = {0,2};
	void init(Level* level, Renderer* renderer);
	int end(Level* level);
	void step(Level* level);
}

#endif