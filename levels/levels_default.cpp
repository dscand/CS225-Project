namespace _level_default {
	const int buttonIndexes[] = {0,2};
	void init(Level* level, Renderer* renderer) {
		//level->renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		level->renderer = renderer;

		// Restart, Exit
		{
			// Restart
			std::string buttonTexturePath = "Textures/Play_Button.png";
			std::string buttonTexturePath_hover = "Textures/Exit_Button.png";
			long double buttonTexScale = 2.;
			int buttonPosX = 420;
			int buttonPosY = 210;
			long double buttonRotation = 0;
			Texture* button = new Texture(level->renderer, buttonTexturePath, buttonTexScale, buttonPosX, buttonPosY, buttonRotation);
			Texture* button_hover = new Texture(level->renderer, buttonTexturePath_hover, buttonTexScale, buttonPosX, buttonPosY, buttonRotation);
			button->draw = false;
			button_hover->draw = false;
			level->textures.push_back(button);
			level->textures.push_back(button_hover);
		}
		{
			// Exit
			std::string buttonTexturePath = "Textures/Exit_Button.png";
			std::string buttonTexturePath_hover = "Textures/Play_Button.png";
			long double buttonTexScale = 2.;
			int buttonPosX = 420;
			int buttonPosY = 310;
			long double buttonRotation = 0;
			Texture* button = new Texture(level->renderer, buttonTexturePath, buttonTexScale, buttonPosX, buttonPosY, buttonRotation);
			Texture* button_hover = new Texture(level->renderer, buttonTexturePath_hover, buttonTexScale, buttonPosX, buttonPosY, buttonRotation);
			button->draw = false;
			button_hover->draw = false;
			level->textures.push_back(button);
			level->textures.push_back(button_hover);
		}
	}

	int end(Level* level) {
		int time = level->gameTime.getTicks();
		level->gameTime.stop();
		return time;
	}

	int close(Level* level) {
		//delete level->renderer;
		//level->renderer = nullptr;
		delete level->background;
		level->background = nullptr;
		delete level->player;
		level->player = nullptr;

		level->textures.clear();
		level->gravityWells_stationary.clear();
		level->gravityWells_moving.clear();
		level->starCoins.clear();

		return level->nextLevel;
	}

	void step(Level* level) {
		float deltaT = level->dtTimer.getTicks() / 1000.f;
		if(level->timeSpeed != 1) {
			deltaT /= level->timeSpeed;
		}
		level->dtTimer.start();

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				// close/X button
				case SDL_QUIT:
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
							for (int buttonIndex : buttonIndexes) {
								level->textures[buttonIndex]->draw = false;
								level->textures[buttonIndex + 1]->draw = false;
							}
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
							level->pause = !level->pause;
							for (int buttonIndex : buttonIndexes) {
								level->textures[buttonIndex]->draw = level->pause;
								level->textures[buttonIndex + 1]->draw = level->pause;
							}
							break;
						case SDL_SCANCODE_1:
						case SDL_SCANCODE_KP_1:
							level->timeSpeed = 1;
							break;
						case SDL_SCANCODE_2:
						case SDL_SCANCODE_KP_2:
							level->timeSpeed = 2;
							break;
						case SDL_SCANCODE_3:
						case SDL_SCANCODE_KP_3:
							level->timeSpeed = 4;
							break;
						case SDL_SCANCODE_4:
						case SDL_SCANCODE_KP_4:
							level->timeSpeed = 8;
							break;

						default:
							break;
					}
					break;

				case SDL_MOUSEMOTION:
					if (level->pause) {
						for (int buttonIndex : buttonIndexes) {
							bool mouseOver = MouseFunctions::mouseOver(level->textures[buttonIndex]->getPosX(), level->textures[buttonIndex]->getPosY(), level->textures[buttonIndex]->getWidth(), level->textures[buttonIndex]->getHeight());
							level->textures[buttonIndex]->draw = !mouseOver;
							level->textures[buttonIndex + 1]->draw = mouseOver;
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (level->pause) {
						for (int buttonIndex : buttonIndexes) {
							if (MouseFunctions::mouseOver(level->textures[buttonIndex]->getPosX(), level->textures[buttonIndex]->getPosY(), level->textures[buttonIndex]->getWidth(), level->textures[buttonIndex]->getHeight())) {
								switch (buttonIndex) {
									case 0:
										// Restart
										level->nextLevel = -2;
										level->stop = true;
										break;

									case 2:
										// Exit
										level->nextLevel = 0;
										level->stop = true;
										break;

									default:
										std::cout << "ERROR: No button function" << std::endl;
										break;
								}
								break;
							}
						}
					}
					break;
					
				default:
					break;
			}
		}

		if (level->pause) {
			// Rendering Step
			level->renderer->clear();

			int centerX = level->renderer->centerXCal(level->player->getPosX(), (level->renderer->getWindowWidth() / 2) + level->player->getOffsetX(), level->getGameWidth() - level->renderer->getWindowWidth());
			int centerY = level->renderer->centerYCal(level->player->getPosY(), (level->renderer->getWindowHeight() / 2) + level->player->getOffsetY(), level->getGameHeight() - level->renderer->getWindowHeight());
			//centerX = level->renderer->centerXCal(level->gravityWells_stationary[0]->getPosX(), (level->renderer->getWindowWidth() / 2) + level->player->getOffsetX(), level->getGameWidth() - level->renderer->getWindowWidth());
			//centerY = level->renderer->centerYCal(level->gravityWells_stationary[0]->getPosY(), (level->renderer->getWindowHeight() / 2) + level->player->getOffsetY(), level->getGameHeight() - level->renderer->getWindowHeight());

			// Render Update
			level->background->render(centerX, centerY);
			for (GravityWell_stationary* gravityWell : level->gravityWells_stationary) {
				gravityWell->render(centerX, centerY);
			}
			for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
				gravityWell->render(centerX, centerY);
			}
			for (StarCoin* starCoin : level->starCoins) {
				starCoin->render(centerX, centerY);
			}
			level->player->render(centerX, centerY);

			for (Texture* texture : level->textures) {
				texture->render(centerX, centerY);
			}

			level->renderer->update();
		}
		else {
			// Control Step
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if(currentKeyStates[SDL_SCANCODE_UP]) {
				level->player->boost(1 * deltaT);
			}
			else {
				level->player->nonboost();
			}
			/*if(currentKeyStates[SDL_SCANCODE_DOWN]) {
				level->player->boost(-1 * deltaT);
			}*/
			if(currentKeyStates[SDL_SCANCODE_RIGHT]) {
				level->player->rotate(1 * deltaT);
			}
			if(currentKeyStates[SDL_SCANCODE_LEFT]) {
				level->player->rotate(-1 * deltaT);
			}


			// Physics Step
			for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
				gravityWell->step(deltaT);
			}
			level->player->playerStep(level->getGameWidth(), level->getGameHeight(), deltaT);

			const int endTime = 4000; // ms
			if (!level->player->isAlive()) {
				if (!level->endTimer.isStarted()) {
					level->nextLevel = 0;
					level->player->explosionIndex = 1;
					level->end();
				} 
				else {
					const int explosionTime = 500; // ms
					int ticks = level->endTimer.getTicks();
					if (ticks > endTime) {
						level->stop = true;
					}
					else if (ticks > 3 * explosionTime) {
						level->player->explosionIndex = 0;
					}
					else if (ticks > 2 * explosionTime) {
						level->player->explosionIndex = 3;
					}
					else if (ticks > explosionTime) {
						level->player->explosionIndex = 2;
					}
				}
			}
			else if (level->ended) {
				int ticks = level->endTimer.getTicks();
				if (ticks > endTime) {
					level->stop = true;
				}
			}


			// Game Step
			const int coinRange = 20;
			for (StarCoin* coin : level->starCoins) {
				long double dist = calcDistance(level->player->getPosX(), level->player->getPosY(), coin->getPosX(), coin->getPosY());
				if (level->player->isAlive() && dist <= coinRange && coin->active) {
					coin->active = false;
					level->score++;

					if (level->scoreGoal > 0 && level->score >= level->scoreGoal) {
						level->end();
					}
				}
			}


			// Rendering Step
			level->renderer->clear();

			int centerX = level->renderer->centerXCal(level->player->getPosX(), (level->renderer->getWindowWidth() / 2) + level->player->getOffsetX(), level->getGameWidth() - level->renderer->getWindowWidth());
			int centerY = level->renderer->centerYCal(level->player->getPosY(), (level->renderer->getWindowHeight() / 2) + level->player->getOffsetY(), level->getGameHeight() - level->renderer->getWindowHeight());
			//centerX = level->renderer->centerXCal(level->gravityWells_stationary[0]->getPosX(), (level->renderer->getWindowWidth() / 2) + level->player->getOffsetX(), level->getGameWidth() - level->renderer->getWindowWidth());
			//centerY = level->renderer->centerYCal(level->gravityWells_stationary[0]->getPosY(), (level->renderer->getWindowHeight() / 2) + level->player->getOffsetY(), level->getGameHeight() - level->renderer->getWindowHeight());

			// Render Update
			level->background->render(centerX, centerY);
			for (GravityWell_stationary* gravityWell : level->gravityWells_stationary) {
				gravityWell->render(centerX, centerY);
			}
			for (GravityWell_moving* gravityWell : level->gravityWells_moving) {
				gravityWell->render(centerX, centerY);
			}
			for (StarCoin* starCoin : level->starCoins) {
				starCoin->render(centerX, centerY);
			}
			for (Texture* texture : level->textures) {
				texture->render(centerX, centerY);
			}
			level->player->render(centerX, centerY);

			level->renderer->update();
		}

		//std::cout << deltaT << std::endl;
		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}
}