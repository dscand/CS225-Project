namespace _level_1 {
	void init(Level* level, Renderer* renderer) {
		level->levelController = new LevelController();

		//level->renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		level->renderer = renderer;
		level->renderer->init();

		{
			std::string backgroundTexturePath = "Textures/background1.png";
			long double imageScale = 2;
			level->background = new Background(level->renderer, backgroundTexturePath, imageScale);
		}

		{
			std::string playerTexturePath = "Textures/rocket_pink.png";
			std::string playerBrokenTexturePath = "Textures/brokenShip_pink.png";
			std::vector<std::string> flameTexturePath = {"Textures/flame.png","Textures/flameLighten.png","Textures/flameDarken.png"};
			long double shipScale = 1;
			long double flameScale = 1;
			long double speed = 100;
			long double rotationSpeed = 540;
			long double rotationalOffset = -45;
			int xRotOffset = 0;
			int yRotOffset = 0;
			level->player = new Player(level->renderer, playerTexturePath, playerBrokenTexturePath, shipScale, flameTexturePath, flameScale, speed, rotationSpeed, rotationalOffset, xRotOffset, yRotOffset);
		}

		//player.setPosX((WINDOW_WIDTH / 2) - player.getOffsetX());
		//player.setPosY((WINDOW_HEIGHT / 2) - player.getOffsetY());
		level->player->setPosX(32);
		level->player->setPosY(32);

		{
			long double magnitude = 10;
			long double edgeMagnitude = 0;
			long double radius = 500;
			std::string texturePath = "Textures/planet3.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_25%.png";
			long double texScale = 2.;
			long double circleTexScale = 1./64.;
			int posX = 512;
			int posY = 512;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			GravityWell_stationary* asteroid1 = new GravityWell_stationary(magnitude, edgeMagnitude, radius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_stationary.push_back(asteroid1);
			level->player->addInfluence(asteroid1);
		}

		{
			//GravityWell_moving asteroid2(5, 0, 300, level->renderer, "Textures/asteroid2.png", "Textures/circle.png", 1, 1./500., 512, 700, -90, 231, 100);
			//level->player->addInfluence(asteroid2);
			//asteroid2.addInfluence(asteroid1);
		}
	}
	int close(Level* level) {
		int gameTime = level->levelController->gameTime.getTicks();

		level->levelController->deathTime.stop();
		level->levelController->gameTime.stop();

		delete level->levelController;
		//delete level->renderer;
		delete level->background;
		delete level->player;

		for (GravityWell_stationary* gravityWell_stationary : level->gravityWells_stationary) { delete gravityWell_stationary; }
		for (GravityWell_moving* gravityWell_moving : level->gravityWells_moving) { delete gravityWell_moving; }

		return gameTime;
	}
	//void step(Level*) {}  use default step
}