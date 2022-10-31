namespace _level_default {
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
					
				default:
					break;
			}
		}

		if (level->pause) {
			// TODO: Pause Menu
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
						//level->stop = true;
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