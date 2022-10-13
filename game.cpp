#include <iostream>

#include "renderer.cpp"

class Background {
	public:
		Background(Renderer&, std::string);
		~Background() { delete texture; }
		void render(int, int);

	private:
		RTexture* texture;
};
Background::Background(Renderer& renderer, std::string texturePath) {
	texture = new RTexture(&renderer);
	texture->loadTexture(texturePath);
}
void Background::render(int windowWidth, int windowHeight) {
	texture->render((windowWidth - texture->getWidth()) / 2, (windowHeight - texture->getHeight()) / 2);
}

class Player {
	public:
		Player(Renderer&, std::string, double, std::string, double, double, double, double);
		~Player() { delete character; delete sprite; }
		void playerStep(int, int, double);
		void render() { character->render(); }
		void boost(int multiplier = 1) { character->addVel(speed * multiplier); }
		void rotate(int multiplier = 1) { character->rotate(rotationSpeed * multiplier); }

	private:
		Character* character;
		Sprite* sprite;

		double speed;
		double rotationSpeed;
		double velocityCap;
};
Player::Player(Renderer& renderer, std::string shipTexturePath, double shipScale, std::string flameTexturePath, double flameScale, double speed = 0.1, double rotationSpeed = 10, double velocityCap = 4) {
	character = new Character(&renderer, velocityCap);
	character->loadTexture(shipTexturePath);
	character->scaleImage(shipScale);

	sprite = new Sprite(&renderer);
	sprite->loadTexture(flameTexturePath);

	this->speed = speed;
	this->rotationSpeed = rotationSpeed;
	this->velocityCap = velocityCap;
}
void Player::playerStep(int windowWidth, int windowHeight, double deltaT = 1.0) {
	character->physicsStep(deltaT);
	sprite->setPos(character->getPosX(), character->getPosY());

	// right boundary
	if (character->getPosX() + character->getWidth() > windowWidth) {
		character->setPosX(windowWidth - character->getWidth());
		character->setVelX(0);
	}

	// left boundary
	if (character->getPosX() < 0) {
		character->setPosX(0);
		character->setVelX(0);
	}

	// bottom boundary
	if (character->getPosY() + character->getHeight() > windowHeight) {
		character->setPosY(windowHeight - character->getHeight());
		character->setVelY(0);
	}

	// upper boundary
	if (character->getPosY() < 0) {
		character->setPosY(0);
		character->setVelY(0);
	}

	// REMOVE
	//std::cout << character->getVel() << std::endl;
}