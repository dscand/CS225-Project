// g++ -std=c++17 main.cpp -I"include" -L"lib" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o Game

#include <iostream>
#include <math.h>

#include "include/SDL.h"
#include "include/SDL_image.h"
//#include "include/SDL_timer.h"

#include "helpers.cpp"
//#include "renderer.cpp"
#include "game.cpp"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int WinMain(int argc, char* argv[]) {
	Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
	renderer.init();

	std::string backgroundTexturePath = "Textures/test2.jpg";
	Background background(renderer, backgroundTexturePath);


	std::string playerTexturePath = "Textures/test.bmp";
	Player player(renderer, playerTexturePath, 1.0/10.0, playerTexturePath, 1.0/10.0);

	//player.moveX((WINDOW_WIDTH - player.getWidth()) / 2);
	//player.moveY((WINDOW_HEIGHT - player.getHeight()) / 2);


	bool stop = false;
	bool pause = false;
	LTimer dtTimer;
	while(!stop) {
		float deltaT = dtTimer.getTicks();
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

						default:
							break;
					}
				
				default:
					break;
			}
		}

		if(pause) {

		}
		else {
			const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
			if(currentKeyStates[SDL_SCANCODE_UP]) {
				player.boost(1);
			}
			/*if(currentKeyStates[SDL_SCANCODE_DOWN]) {
				player.boost(-1);
			}*/
			if(currentKeyStates[SDL_SCANCODE_RIGHT]) {
				player.rotate(1);
			}
			if(currentKeyStates[SDL_SCANCODE_LEFT]) {
				player.rotate(-1);
			}


			player.playerStep(WINDOW_WIDTH, WINDOW_HEIGHT, deltaT);


			renderer.clear();

			// Texture Update
			background.render(WINDOW_WIDTH, WINDOW_HEIGHT);
			player.render();

			renderer.update();
		}

		std::cout << deltaT << std::endl;
		// calculates to 60 fps
		//SDL_Delay(1000 / 60);
	}

	return 0;
}