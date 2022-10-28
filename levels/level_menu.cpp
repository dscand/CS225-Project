namespace _level_menu {
	void init(Level* level, Renderer* renderer) {
		//level->renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		level->renderer = renderer;

		level->setGameWidth(level->renderer->getWindowWidth());
		level->setGameHeight(level->renderer->getWindowHeight());

		{
			std::string backgroundTexturePath = "Textures/background4.png";
			long double imageScale = (1./1024.)*(long double)level->renderer->getWindowWidth();
			level->background = new Background(level->renderer, backgroundTexturePath, imageScale);
		}

		{
			long double magnitude = 10;
			long double radius = 500;
			std::string texturePath = "Textures/planet1.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_25%.png";
			long double texScale = (1./126.)*(long double)level->renderer->getWindowWidth()*0.25;
			long double circleTexScale = 1./64.;
			int posX = level->renderer->getWindowWidth()*0.5;
			int posY = level->renderer->getWindowHeight()*0.7;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			GravityWell_stationary* object = new GravityWell_stationary(magnitude, radius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_stationary.push_back(object);
		}

	}
	int end(Level* level) { return 0; }
	void close(Level* level) {
		//delete level->renderer;
		//level->renderer = nullptr;
		delete level->background;
		level->background = nullptr;

		level->textures.clear();
		level->gravityWells_stationary.clear();
		level->gravityWells_moving.clear();
	}
	void step(Level* level) {
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
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_CLOSE:
							level->stop = true;
							break;
						case SDL_WINDOWEVENT_MINIMIZED:
						case SDL_WINDOWEVENT_FOCUS_LOST:
							level->pause = true;
							break;
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							level->renderer->setWindow(event.window.data1, event.window.data2);
							break;

						default:
							break;
					}

				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
						default:
							break;
					}

				case SDL_MOUSEMOTION:
					//int x, y;
					//SDL_GetMouseState( &x, &y );
					break;
				case SDL_MOUSEBUTTONDOWN:
					//int x, y;
					//SDL_GetMouseState( &x, &y );
					break;
				case SDL_MOUSEBUTTONUP:
					//int x, y;
					//SDL_GetMouseState( &x, &y );
					break;
				
				default:
					break;
			}
		}



		// Physics Update
		for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
			gravityWell->step(deltaT);
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
		
		//std::cout << deltaT << std::endl;
		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}
}
