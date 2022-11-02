namespace _level_1 {
	void init(Level* level, Renderer* renderer) {
		_level_default::init(level, renderer);

		level->setGameWidth(1200);
		level->setGameHeight(1200);

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
			long double collisionRadius = 90;
			std::string texturePath = "Textures/planet1.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_25%.png";
			long double texScale = 2.;
			long double circleTexScale = 1./64.;
			int posX = 500;
			int posY = 500;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			GravityWell_stationary* object = new GravityWell_stationary(magnitude, radius, collisionRadius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_stationary.push_back(object);
			level->player->addInfluence(object);
		}
		{
			long double magnitude = 5;
			long double radius = 300;
			long double collisionRadius = 50;
			std::string texturePath = "Textures/asteroid1.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_10%.png";
			long double texScale = 1.;
			long double circleTexScale = 1./64.;
			int posX = 900;
			int posY = 900;
			long double rotation = 52;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			GravityWell_stationary* object = new GravityWell_stationary(magnitude, radius, collisionRadius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_stationary.push_back(object);
			level->player->addInfluence(object);
		}

		{
			std::vector<std::string> texturePath = {"Textures/star_coin_1.png", "Textures/star_coin_2.png"};
			long double texScale = 2.;
			int posX = 300;
			int posY = 300;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			StarCoin* object = new StarCoin(level->renderer, texturePath, texScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->starCoins.push_back(object);
		}
		{
			std::vector<std::string> texturePath = {"Textures/star_coin_1.png", "Textures/star_coin_2.png"};
			long double texScale = 2.;
			int posX = 300;
			int posY = 700;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			StarCoin* object = new StarCoin(level->renderer, texturePath, texScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->starCoins.push_back(object);
		}
		{
			std::vector<std::string> texturePath = {"Textures/star_coin_1.png", "Textures/star_coin_2.png"};
			long double texScale = 2.;
			int posX = 700;
			int posY = 300;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			StarCoin* object = new StarCoin(level->renderer, texturePath, texScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->starCoins.push_back(object);
		}
		{
			std::vector<std::string> texturePath = {"Textures/star_coin_1.png", "Textures/star_coin_2.png"};
			long double texScale = 2.;
			int posX = 800;
			int posY = 800;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			StarCoin* object = new StarCoin(level->renderer, texturePath, texScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->starCoins.push_back(object);
		}
	}
	//int end(Level* level) {}  use default end
	//int close(Level* level) {}  use default close
	//void step(Level*) {}  use default step
}