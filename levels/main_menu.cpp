#include "menu.cpp"

void gameLevelStep(Level* level) {
	float deltaT = level->dtTimer.getTicks() / 1000.f;
	if(level->timeSpeed != 1) {
		deltaT /= level->timeSpeed;
	}
	level->dtTimer.start();

	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				// handling of close button
				level->stop = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						level->pause = !level->pause;
						break;
					
					default:
						break;
				}
			case SDL_MOUSEMOTION:
				int x, y;
        			SDL_GetMouseState( &x, &y );
				break;
			case SDL_MOUSEBUTTONDOWN:
				int x, y;
        			SDL_GetMouseState( &x, &y );
				break;
			case SDL_SDL_MOUSEBUTTONUP:
				int x, y;
        			SDL_GetMouseState( &x, &y );
				break;
			
			default:
				break;
		}
	}



	// Physics Update
	for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
		gravityWell->step(level->GAME_WIDTH, level->GAME_HEIGHT, deltaT);
	}

	level->renderer->clear();


	// Render Update
	level->background->render(0, 0);
	for (GravityWell_stationary* gravityWell : level->gravityWells_stationary) {
			gravityWell->render(0, 0);
	}
	for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
			gravityWell->render(0, 0);
	}

	level->renderer->update();
}

	//std::cout << deltaT << std::endl;
	// calculates to 60 fps
	SDL_Delay(1000 / 60);
}
	
namespace _level_menu {
	void init(Level* level, Renderer* renderer) {
		//level->renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		level->renderer = renderer;

		{
			std::string backgroundTexturePath = "Textures/background4.png";
			long double imageScale = 2;
			level->background = new Background(level->renderer, backgroundTexturePath, imageScale);
		}

		{
			long double magnitude = 10;
			long double radius = 500;
			std::string texturePath = "Textures/planet4.png";
			long double texScale = 2.;
			int posX = 1012;
			int posY = 512;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			GravityWell_stationary* asteroid1 = new GravityWell_stationary(magnitude, radius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_stationary.push_back(asteroid1);
		}

	}
	int end(Level* level) {
		int time = 0;
	}
	void close(Level* level) {
		//delete level->renderer;
		//level->renderer = nullptr;
		delete level->background;
		level->background = nullptr;

		level->gravityWells_stationary.clear();
		level->gravityWells_moving.clear();
	}
	//void step(Level*) {}  use default step
}
