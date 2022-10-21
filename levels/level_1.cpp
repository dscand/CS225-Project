namespace _level_1 {
	void init(Level* level) {
		const int WINDOW_WIDTH = 840;
		const int WINDOW_HEIGHT = 840;
		level->renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		level->renderer->init();

		std::string backgroundTexturePath = "Textures/background1.png";
		level->background = new Background(level->renderer, backgroundTexturePath, 2);


		std::string playerTexturePath = "Textures/rocket_pink.png";
		std::vector<std::string> flameTexturePath = {"Textures/flame.png","Textures/flameLighten.png","Textures/flameDarken.png"};
		level->player = new Player(level->renderer, playerTexturePath, 1, flameTexturePath, 1, 100, 540, 4, -45);

		//player.setPosX((WINDOW_WIDTH / 2) - player.getOffsetX());
		//player.setPosY((WINDOW_HEIGHT / 2) - player.getOffsetY());
		level->player->setPosX(0);
		level->player->setPosY(0);

		
		GravityWell_stationary* asteroid1 = new GravityWell_stationary(10, 0, 500, level->renderer, "Textures/planet3.png", "Textures/Aura_of_Influence_25%.png", 2., 1./64., 512, 512, 0);
		level->gravityWells_stationary.push_back(asteroid1);
		level->player->addInfluence(asteroid1);

		//GravityWell_moving asteroid2(5, 0, 300, level->renderer, "Textures/asteroid2.png", "Textures/circle.png", 1, 1./500., 512, 700, -90, 231, 100);
		//level->player->addInfluence(asteroid2);
		//asteroid2.addInfluence(asteroid1);
	}
	void close(Level* level) {}
	//void step(Level*, long double) {}  use default step
}