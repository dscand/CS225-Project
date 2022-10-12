// g++ -std=c++17 main.cpp -I"include" -L"lib" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o Game

#include <iostream>
#include <math.h>

#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_timer.h"

#include "renderer.cpp"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int WinMain(int argc, char* argv[]) {
	Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
	renderer.init();


	RTexture background(&renderer);
	std::string backgroundTexturePath = "Textures/test2.jpg";
	//background.loadTexture(backgroundTexturePath);
	background.scaleImage(1.0);


	double speed = 0.1;
	double rotationSpeed = 10;
	double velocityCap = 4;

	Character player(&renderer, velocityCap);
	std::string playerTexturePath = "Textures/test.bmp";
	player.loadTexture(playerTexturePath);
	player.scaleImage(1.0/10.0);

	player.moveX((WINDOW_WIDTH - player.getWidth()) / 2);
	player.moveY((WINDOW_HEIGHT - player.getHeight()) / 2);


	bool stop = false;
	bool pause = false;
	while(!stop) {
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

		if(!pause) {
		
			const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
			if( currentKeyStates[ SDL_SCANCODE_UP ] ) {
				player.addVel(speed);
			}
			/*if( currentKeyStates[ SDL_SCANCODE_DOWN ] ) {
				player.addVel(-speed);
			}*/
			if( currentKeyStates[ SDL_SCANCODE_RIGHT ] ) {
				player.rotate(rotationSpeed);
			}
			if( currentKeyStates[ SDL_SCANCODE_LEFT ] ) {
				player.rotate(-rotationSpeed);
			}


			player.physicsStep();

			// right boundary
			if (player.getPosX() + player.getWidth() > WINDOW_WIDTH) {
				player.setPosX(WINDOW_WIDTH - player.getWidth());
				player.setVelX(0);
			}

			// left boundary
			if (player.getPosX() < 0) {
				player.setPosX(0);
				player.setVelX(0);
			}

			// bottom boundary
			if (player.getPosY() + player.getHeight() > WINDOW_HEIGHT) {
				player.setPosY(WINDOW_HEIGHT - player.getHeight());
				player.setVelY(0);
			}

			// upper boundary
			if (player.getPosY() < 0) {
				player.setPosY(0);
				player.setVelY(0);
			}


			renderer.clear();

			// Texture Update
			background.render((WINDOW_WIDTH - background.getWidth()) / 2, (WINDOW_HEIGHT - background.getHeight()) / 2);
			player.render();

			renderer.update();

			std::cout << player.getVel() << std::endl;
		}		

		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}

	player.free();

	renderer.close();
	return 0;
}