#include "level_3.hpp"

void _level_3::init(Level* level, Renderer* renderer) {
		_level_default::init(level, renderer);

		level->setGameWidth(2000);
		level->setGameHeight(2000);

		{
			std::string backgroundTexturePath = "Textures/background3.png";
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
		level->player->setPosX(1200);
		level->player->setPosY(1000);
		level->player->setVel(0, 305);

		{
			long double magnitude = 20;
			long double radius = 1000;
			long double collisionRadius = 50;
			std::string texturePath = "Textures/planet3.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_25%.png";
			long double texScale = 2.;
			long double circleTexScale = 1./64.;
			int posX = 1000;
			int posY = 1000;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			GravityWell_stationary* object = new GravityWell_stationary(magnitude, radius, collisionRadius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_stationary.push_back(object);
			level->player->addInfluence(object);
		}

		{
			long double magnitude = 20;
			long double radius = 200;
			long double collisionRadius = 50;
			std::string texturePath = "Textures/planet_black.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_50%.png";
			long double texScale = 1;
			long double circleTexScale = 1./64.; // will scale by radius
			int posX = 1000;
			int posY = 250;
			long double rotation = -90;
			long double velocity = 90;
			long double rotationalVelocity = 100;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			GravityWell_moving* object = new GravityWell_moving(magnitude, radius, collisionRadius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, velocity, rotationalVelocity, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_moving.push_back(object);
			level->player->addInfluence(object);
			object->addInfluence(level->gravityWells_stationary[0]);
		}
		{
			long double magnitude = -20;
			long double radius = 200;
			long double collisionRadius = 50;
			std::string texturePath = "Textures/planet_white.png";
			std::string circleTexturePath = "Textures/Aura_of_Influence_50%.png";
			long double texScale = 1;
			long double circleTexScale = 1./64.; // will scale by radius
			int posX = 1000;
			int posY = 1750;
			long double rotation = -90;
			long double velocity = 110;
			long double rotationalVelocity = 100;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			GravityWell_moving* object = new GravityWell_moving(magnitude, radius, collisionRadius, level->renderer, texturePath, circleTexturePath, texScale, circleTexScale, posX, posY, rotation, velocity, rotationalVelocity, rotationalOffset, xRotOffset, yRotOffset);
			level->gravityWells_moving.push_back(object);
			level->player->addInfluence(object);
			object->addInfluence(level->gravityWells_stationary[0]);
		}

		{
			std::vector<std::string> texturePath = {"Textures/star_coin_3.png", "Textures/star_coin_3_sparkle.png"};
			long double texScale = 2.;
			int posX = 1400;
			int posY = 1000;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			StarCoin* object = new StarCoin(level->renderer, texturePath, texScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->starCoins.push_back(object);
		}
		{
			std::vector<std::string> texturePath = {"Textures/star_coin_3.png", "Textures/star_coin_3_sparkle.png"};
			long double texScale = 2.;
			int posX = 1000;
			int posY = 1400;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			StarCoin* object = new StarCoin(level->renderer, texturePath, texScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->starCoins.push_back(object);
		}
		{
			std::vector<std::string> texturePath = {"Textures/star_coin_3.png", "Textures/star_coin_3_sparkle.png"};
			long double texScale = 2.;
			int posX = 1000;
			int posY = 600;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			StarCoin* object = new StarCoin(level->renderer, texturePath, texScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->starCoins.push_back(object);
		}
		{
			std::vector<std::string> texturePath = {"Textures/star_coin_3.png", "Textures/star_coin_3_sparkle.png"};
			long double texScale = 2.;
			int posX = 600;
			int posY = 1000;
			long double rotation = 0;
			long double rotationalOffset = 0;
			int xRotOffset = 0;
			int yRotOffset = 0;
			StarCoin* object = new StarCoin(level->renderer, texturePath, texScale, posX, posY, rotation, rotationalOffset, xRotOffset, yRotOffset);
			level->starCoins.push_back(object);
		}
	}
