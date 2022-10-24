namespace _level_1 {
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
