namespace _level_1 {
	void init(Level* level, Renderer* renderer) {
		//level->renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		level->renderer = renderer;

		level->setGameWidth(2048);
		level->setGameHeight(2048);

		{
			std::string backgroundTexturePath = "Textures/background1.png";
			long double imageScale = (1./1024.)*(long double)level->getGameWidth();
			level->background = new Background(level->renderer, backgroundTexturePath, imageScale);
		}

		{
			std::string playerTexturePath = "Textures/rocket_pink.png";
			std::string playerBrokenTexturePath = "Textures/brokenShip_pink.png";
			std::vector<std::string> flameTexturePath = {"Textures/flame.png","Textures/flame_lighten.png","Textures/flame_darken.png"};
			std::vector<std::string> explosion1TexturePath = {"Textures/explosion1.png","Textures/explosion1_lighten.png","Textures/explosion1_darken.png"};
			std::vector<std::string> explosion2TexturePath = {"Textures/explosion2.png","Textures/explosion2_lighten.png","Textures/explosion2_darken.png"};
			std::vector<std::string> explosion3TexturePath = {"Textures/explosion3.png","Textures/explosion3_lighten.png","Textures/explosion3_darken.png"};
			long double shipScale = 1;
			long double flameScale = 1;
			long double explosionScale = 1;
			long double speed = 100;
			long double rotationSpeed = 540;
			long double rotationalOffset = -45;
			int xRotOffset = 0;
			int yRotOffset = 0;
			level->player = new Player(level->renderer, playerTexturePath, playerBrokenTexturePath, shipScale, flameTexturePath, flameScale, explosion1TexturePath, explosion2TexturePath, explosion3TexturePath, explosionScale, speed, rotationSpeed, rotationalOffset, xRotOffset, yRotOffset);
		}

		//player.setPosX((WINDOW_WIDTH / 2) - player.getOffsetX());
		//player.setPosY((WINDOW_HEIGHT / 2) - player.getOffsetY());
		level->player->setPosX(32);
		level->player->setPosY(32);

		{
			long double magnitude = 10;
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
			GravityWell_stationary* object = new GravityWell_stationary(magnitude, radius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_stationary.push_back(object);
			level->player->addInfluence(object);
		}

		{
			long double magnitude = 5;
			long double radius = 300;
			std::string texturePath = "Textures/asteroid2.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_10%.png";
			long double texScale = 1;
			long double circleTexScale = 1./64.; // will scale by radius
			int posX = 512;
			int posY = 700;
			long double rotation = -90;
			long double velocity = 231; // TODO: calculate perfect velocity for orbit
			long double rotationalVelocity = 100;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			GravityWell_moving* object = new GravityWell_moving(magnitude, radius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, velocity, rotationalVelocity, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_moving.push_back(object);
			level->player->addInfluence(object);
			object->addInfluence(level->gravityWells_stationary[0]);
		}

		{
			std::vector<std::string> texturePath = {"Textures/star_coin_1.png", "Textures/star_coin_2.png"};
			long double texScale = 2.;
			int posX = 1200;
			int posY = 1200;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			StarCoin* object = new StarCoin(level->renderer, texturePath, texScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->starCoins.push_back(object);
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

		level->textures.clear();
		level->gravityWells_stationary.clear();
		level->gravityWells_moving.clear();
	}
	//void step(Level*) {}  use default step
}