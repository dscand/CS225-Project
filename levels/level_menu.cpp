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

		// Play, Exit
		{
			// Play
			std::string buttonTexturePath = "Textures/Play_Button.png";
			long double buttonTexScale = 2.;
			int buttonPosX = 420;
			int buttonPosY = 210;
			long double buttonRotation = 0;
			Texture* button = new Texture(level->renderer, buttonTexturePath, buttonTexScale, buttonPosX, buttonPosY, buttonRotation); 
			level->textures.push_back(button);
		}
		{
			// Exit
			std::string buttonTexturePath = "Textures/Exit_Button.png";
			long double buttonTexScale = 2.;
			int buttonPosX = 420;
			int buttonPosY = 310;
			long double buttonRotation = 0;
			Texture* button = new Texture(level->renderer, buttonTexturePath, buttonTexScale, buttonPosX, buttonPosY, buttonRotation); 
			level->textures.push_back(button);
		}

		{
			long double magnitude = 10;
			long double radius = 500;
			std::string texturePath = "Textures/planet4.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_25%.png";
			long double texScale = (1./126.)*(long double)level->renderer->getWindowWidth()*0.25;
			long double circleTexScale = 0.;
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
	#include "../menu.cpp"
	void step(Level* level) {
		float deltaT = level->dtTimer.getTicks() / 1000.f;
		if(level->timeSpeed != 1) {
			deltaT /= level->timeSpeed;
		}
		level->dtTimer.start();

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
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
						case SDL_SCANCODE_ESCAPE:
							level->nextLevel = -1;
							level->stop = true;
							break;
						case SDL_SCANCODE_1:
							level->nextLevel = 1;
							level->stop = true;
							break;
						case SDL_SCANCODE_2:
							level->nextLevel = 2;
							level->stop = true;
							break;
						case SDL_SCANCODE_3:
							level->nextLevel = 3;
							level->stop = true;
							break;
						default:
							break;
					}

				case SDL_MOUSEMOTION:
					for (Texture* texture : level->textures) {
						if (MouseFunctions::mouseOver(texture->getPosX(), texture->getPosY(), texture->getWidth(), texture->getHeight())) {
							// TODO: mouse hover function
							break;
						}
						else {
							// TODO: mouse not hover function
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					for (Texture* texture : level->textures) {
						if (MouseFunctions::mouseOver(texture->getPosX(), texture->getPosY(), texture->getWidth(), texture->getHeight())) {
							// TODO: mouse button down function
							break;
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					for (Texture* texture : level->textures) {
						if (MouseFunctions::mouseOver(texture->getPosX(), texture->getPosY(), texture->getWidth(), texture->getHeight())) {
							// TODO: mouse button up function
							break;
						}
					}
					break;
				
				default:
					break;
			}
		}


		// Physics Update
		for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
			gravityWell->step(deltaT);
		}


		// Render Update
		level->renderer->clear();

		level->background->render(0, 0);
		for (GravityWell_stationary* gravityWell : level->gravityWells_stationary) {
				gravityWell->render(0, 0);
		}
		for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
				gravityWell->render(0, 0);
		}
		for (StarCoin* starCoin : level->starCoins) {
			starCoin->render(0, 0);
		}
		for (Texture* texture : level->textures) {
			texture->render(0, 0);
		}

		level->renderer->update();
		
		//std::cout << deltaT << std::endl;
		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}
}
