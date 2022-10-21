// g++ -std=c++17 main.cpp -I"include" -L"lib" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o Game

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>

#include "include/SDL.h"
#include "include/SDL_image.h"
//#include "include/SDL_timer.h"

#include "helpers.cpp"
//#include "renderer.cpp"
#include "game.cpp"
#include "levels.cpp"

const int WINDOW_WIDTH = 840;
const int WINDOW_HEIGHT = 840;

int WinMain(int argc, char* argv[]) {
	std::cout << "Starting" << std::endl;
	srand(time(NULL));

	Level level = level_1;
	level.init();


	bool stop = false;
	bool pause = false;
	float timeSpeed = 1;
	LTimer dtTimer;
	while(!stop) {
		float deltaT = dtTimer.getTicks() / 1000.f / timeSpeed;
		dtTimer.start();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					// handling of close button
					stop = true;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
						case SDL_SCANCODE_ESCAPE:
							pause = !pause;
							break;
						case SDL_SCANCODE_1:
						case SDL_SCANCODE_KP_1:
							timeSpeed = 1;
							break;
						case SDL_SCANCODE_2:
						case SDL_SCANCODE_KP_2:
							timeSpeed = 2;
							break;
						case SDL_SCANCODE_3:
						case SDL_SCANCODE_KP_3:
							timeSpeed = 4;
							break;
						case SDL_SCANCODE_4:
						case SDL_SCANCODE_KP_4:
							timeSpeed = 8;
							break;

						default:
							break;
					}
				
				default:
					break;
			}
		}

		if(pause) {
			// TODO: Pause Menu
		}
		else {
			level.step(deltaT);
		}

		//std::cout << deltaT << std::endl;
		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}

	std::cout << "Exitting" << std::endl;
	return 0;
}