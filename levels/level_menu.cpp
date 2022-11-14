#include "level_menu.hpp"

void _level_menu::init(Level* level, Renderer* renderer) {
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
		std::string buttonTexturePath_hover = "Textures/Play_Button_Lighten.png";
		long double buttonTexScale = 2.;
		int buttonPosX = level->renderer->getWindowWidth()*0.5;
		int buttonPosY = level->renderer->getWindowHeight()*0.65;
		long double buttonRotation = 0;
		Texture* button = new Texture(level->renderer, buttonTexturePath, buttonTexScale, buttonPosX, buttonPosY, buttonRotation);
		Texture* button_hover = new Texture(level->renderer, buttonTexturePath_hover, buttonTexScale, buttonPosX, buttonPosY, buttonRotation);
		button_hover->draw = false;
		level->textures.push_back(button);
		level->textures.push_back(button_hover);
	}
	{
		// Exit
		std::string buttonTexturePath = "Textures/Exit_Button.png";
		std::string buttonTexturePath_hover = "Textures/Exit_Button_Lighten.png";
		long double buttonTexScale = 2.;
		int buttonPosX = level->renderer->getWindowWidth()*0.5;
		int buttonPosY = level->renderer->getWindowHeight()*0.8;
		long double buttonRotation = 0;
		Texture* button = new Texture(level->renderer, buttonTexturePath, buttonTexScale, buttonPosX, buttonPosY, buttonRotation);
		Texture* button_hover = new Texture(level->renderer, buttonTexturePath_hover, buttonTexScale, buttonPosX, buttonPosY, buttonRotation);
		button_hover->draw = false;
		level->textures.push_back(button);
		level->textures.push_back(button_hover);
	}

	{
		long double magnitude = 10;
		long double radius = 500;
		long double collisionRadius = 50;
		std::string texturePath = "Textures/planet4.png";
		std::string circleTexturePath = "Textures/Aura_of_Influence_25%.png";
		long double texScale = (1./126.)*(long double)level->renderer->getWindowWidth()*0.25;
		long double circleTexScale = 0.;
		int posX = level->renderer->getWindowWidth()*0.75;
		int posY = level->renderer->getWindowHeight()*0.7;
		long double rotation = 0;
		long double rotationalOffset = 0;
		int xRotOffset = 0;
		int yRotOffset = 0;
		GravityWell_stationary* object = new GravityWell_stationary(magnitude, radius, collisionRadius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
		level->gravityWells_stationary.push_back(object);
	}
		
	{
		std::string texturePath = "Textures/brokenShip.png";
		long double texScale = (1./64.)*(long double)level->renderer->getWindowWidth()*0.25;
		int posX = level->renderer->getWindowWidth()*0.2;
		int posY = level->renderer->getWindowHeight()*0.4;
		long double rotation = 0;
		Texture* ship = new Texture(level->renderer, texturePath, texScale, posX, posY, rotation);
		level->textures.push_back(ship);
	}

	{
		std::string texturePath = "Textures/Title.png";
		long double texScale = (1./64.)*(long double)level->renderer->getWindowWidth()*0.25;
		int posX = level->renderer->getWindowWidth()*0.5;
		int posY = level->renderer->getWindowHeight()*0.2;
		long double rotation = 0;
		Texture* ship = new Texture(level->renderer, texturePath, texScale, posX, posY, rotation);
		level->textures.push_back(ship);
	}
}
int _level_menu::end(Level* level) { return 0; }
void _level_menu::step(Level* level) {
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
				break;

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
				break;

			case SDL_MOUSEMOTION:
				for (int buttonIndex : buttonIndexes) {
					bool mouseOver = MouseFunctions::mouseOver(level->textures[buttonIndex]->getPosX(), level->textures[buttonIndex]->getPosY(), level->textures[buttonIndex]->getWidth(), level->textures[buttonIndex]->getHeight());
					level->textures[buttonIndex]->draw = !mouseOver;
					level->textures[buttonIndex + 1]->draw = mouseOver;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				for (int buttonIndex : buttonIndexes) {
					if (MouseFunctions::mouseOver(level->textures[buttonIndex]->getPosX(), level->textures[buttonIndex]->getPosY(), level->textures[buttonIndex]->getWidth(), level->textures[buttonIndex]->getHeight())) {
						switch (buttonIndex) {
							case 0:
								// Play Sound effect
								
								// Play
								level->nextLevel = 1;
								level->stop = true;
								break;

							case 2:
								// Play Sound effect
								
								// Exit
								level->nextLevel = -1;
								level->stop = true;
								break;

							default:
								std::cout << "ERROR: No button function" << std::endl;
								break;
						}
						break;
					}
				}
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
