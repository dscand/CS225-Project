namespace _level_1 {
	void init(Level* level, Renderer* renderer) {
		//level->renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		level->renderer = renderer;

		{
			std::string backgroundTexturePath = "Textures/background1.png";
			long double imageScale = 2;
			level->background = new Background(level->renderer, backgroundTexturePath, imageScale);
		}

		{
			long double magnitude = 10;
			long double radius = 500;
			std::string texturePath = "Textures/planet4.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_25%.png";
			long double texScale = 2.;
			long double circleTexScale = 1./64.;
			int posX = 512;
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
		int time = level->gameTime.getTicks();
		level->gameTime.stop();
		return time;
	}
	void close(Level* level) {
		//delete level->renderer;
		//level->renderer = nullptr;
		delete level->background;
		level->background = nullptr;
		delete level->player;
		level->player = nullptr;

		level->gravityWells_stationary.clear();
		level->gravityWells_moving.clear();
	}
	//void step(Level*) {}  use default step
}
