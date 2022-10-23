#ifndef game
#define game

#include <iostream>

#include "renderer.cpp"

class Background {
	public:
		Background(Renderer*, std::string, long double);
		~Background() { delete texture; }
		void render(int, int);

	private:
		RTexture* texture;
};
Background::Background(Renderer* renderer, std::string texturePath, long double imageScale = 1) {
	texture = new RTexture(renderer);
	texture->loadTexture(texturePath);
	texture->scaleImage(imageScale);
}
void Background::render(int windowOffsetX = 0, int windowOffsetY = 0) {
	texture->render(windowOffsetX, windowOffsetY);
}

class Player {
	public:
		Player(Renderer*, std::string, std::string, long double, std::vector<std::string>, long double, long double, long double, long double, int, int);
		~Player() { delete object; for (Sprite* each : sprite) delete each; }
		void playerStep(int, int, long double);
		void render(int, int, long double);
		void boost(long double multiplier = 1) { if (alive) object->addVel(speed * multiplier); boosting = true; }
		void nonboost() { boosting = false; }
		void rotate(long double multiplier = 1) { if (alive) object->rotate(rotationSpeed * multiplier); }
		void setPosX(long double posX) { object->setPosX(posX); }
		void setPosY(long double posY) { object->setPosY(posY); }
		long double getPosX() { return object->getPosX(); }
		long double getPosY() { return object->getPosY(); }
		long double getOffsetX() { return object->getWidth() / 2; }
		long double getOffsetY() { return object->getHeight() / 2; }
		long double getVel() { return object->getVel(); }
		void addInfluence(GravityWell* influence) { object->addInfluence(influence); }
		bool isAlive() { return alive; }

	private:
		Object* object;
		Sprite* spriteBroken;
		std::vector<Sprite*> sprite;

		bool alive;
		bool boosting;
		int flameSel;

		long double speed;
		long double rotationSpeed;
		long double velocityCap;

		int xRotOffset;
		int yRotOffset;
		long double rotationalOffset;
};
Player::Player(Renderer* renderer, std::string shipTexturePath, std::string shipBrokenTexturePath, long double shipScale, std::vector<std::string> flameTexturePath, long double flameScale, long double speed = 10, long double rotationSpeed = 180, long double rotationalOffset = 0, int xRotOffset = 0, int yRotOffset = 0) {
	object = new Object(renderer);
	object->loadTexture(shipTexturePath);
	object->scaleImage(shipScale);

	spriteBroken = new Sprite(renderer);
	spriteBroken->loadTexture(shipBrokenTexturePath);
	spriteBroken->scaleImage(shipScale);

	for (std::string path : flameTexturePath) {
		Sprite* _sprite = new Sprite(renderer);
		_sprite->loadTexture(path);
		_sprite->scaleImage(flameScale);
		sprite.push_back(_sprite);
	}

	alive = true;
	boosting = false;
	flameSel = 0;

	this->speed = speed;
	this->rotationSpeed = rotationSpeed;
	this->velocityCap = velocityCap;

	this->rotationalOffset = rotationalOffset;
	this->xRotOffset = xRotOffset;
	this->yRotOffset = yRotOffset;
}
void Player::render(int windowOffsetX = 0, int windowOffsetY = 0, long double rotationOffset = 0) {
	if (alive)
		object->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
	else
		spriteBroken->render(windowOffsetX, windowOffsetY, object->getRot() + rotationalOffset);

	if (boosting && alive) {
		sprite.at(flameSel)->render(windowOffsetX, windowOffsetY, object->getRot() + rotationalOffset);
		flameSel = rand() % sprite.size();
	}
}
void Player::playerStep(int gameWidth, int gameHeight, long double deltaT = 1.0) {
	object->physicsStep(deltaT);
	spriteBroken->setPos(object->getPosX(), object->getPosY());
	sprite.at(flameSel)->setPos(object->getPosX(), object->getPosY());

	/* if (!alive) {
		const double decay = 0.96;

		if (abs(object->getVelX()) > 1) object->setVelX(object->getVelX() * decay);
		else object->setVelX(0);
		
		if (abs(object->getVelY()) > 1) object->setVelY(object->getVelY() * decay);
		else object->setVelY(0);
	} */

	// right boundary
	if (object->getPosX() > gameWidth) {
		object->setPosX((2 * gameWidth) - object->getPosX());
		object->setVelX(-object->getVelX());
		alive = false;
	}

	// left boundary
	if (object->getPosX() < 0) {
		object->setPosX(-object->getPosX());
		object->setVelX(-object->getVelX());
		alive = false;
	}

	// bottom boundary
	if (object->getPosY() > gameHeight) {
		object->setPosY((2 * gameHeight) - object->getPosY());
		object->setVelY(-object->getVelY());
		alive = false;
	}

	// upper boundary
	if (object->getPosY() < 0) {
		object->setPosY(-object->getPosY());
		object->setVelY(-object->getVelY());
		alive = false;
	}

	// REMOVE
	//std::cout << object->getVel() << std::endl;
	//std::cout << object->getVelX() << ", " << object->getVelY() << std::endl;
}


class GravityWell_stationary : public GravityWell {
	public:
		GravityWell_stationary(long double, long double, long double, Renderer*, std::string, std::string, long double, long double, int, int, long double, long double, int, int);
		~GravityWell_stationary() { delete sprite; delete circle; }
		void render(int, int, long double);
		long double calcGravityMag(int posX, int posY) { return calcGravityWellMag(this->posX, this->posY, posX, posY); }
		long double calcGravityRot(int posX, int posY) { return calcGravityWellRot(this->posX, this->posY, posX, posY); }

	private:
		Sprite* sprite;
		Sprite* circle;

		int posX;
		int posY;
		long double rotation;

		int xRotOffset;
		int yRotOffset;
		long double rotationalOffset;
};
GravityWell_stationary::GravityWell_stationary(long double magnitude, long double edgeMagnitude, long double radius, Renderer* renderer, std::string texturePath, std::string circleTexturePath, long double texScale, long double circleTexScale, int posX, int posY, long double rotation, long double rotationalOffset = 0, int xRotOffset = 0, int yRotOffset = 0)
	: GravityWell(magnitude, edgeMagnitude, radius)
{
	sprite = new Sprite(renderer);
	sprite->loadTexture(texturePath);
	sprite->scaleImage(texScale);

	circle = new Sprite(renderer);
	circle->loadTexture(circleTexturePath);
	circle->scaleImage(circleTexScale * radius);

	this->posX = posX;
	this->posY = posY;
	this->rotation = rotation;
	sprite->setPos(posX, posY);
	sprite->setRot(rotation);
	circle->setPos(posX, posY);
	circle->setRot(rotation);

	this->rotationalOffset = rotationalOffset;
	this->xRotOffset = xRotOffset;
	this->yRotOffset = yRotOffset;
}
void GravityWell_stationary::render(int windowOffsetX = 0, int windowOffsetY = 0, long double rotationOffset = 0) {
	circle->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
	sprite->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
}

class GravityWell_moving : public GravityWell {
	public:
		GravityWell_moving(long double, long double, long double, Renderer*, std::string, std::string, long double, long double, int, int, long double, long double, long double, long double, int, int);
		~GravityWell_moving() { delete object; delete circle; }
		void step(int, int, long double);
		void render(int, int, long double);
		long double calcGravityMag(int posX, int posY) { std::cout << "GravityWell_moving, calcGravityMag" << std::endl; return calcGravityWellMag(object->getPosX(), object->getPosY(), posX, posY); }
		long double calcGravityRot(int posX, int posY) { std::cout << "GravityWell_moving, calcGravityRot" << std::endl; return calcGravityWellRot(object->getPosX(), object->getPosY(), posX, posY); }
		void setVel(long double vel) { object->setVel(vel); }
		void addInfluence(GravityWell* influence) { object->addInfluence(influence); }
	
	private:
		Object* object;
		Sprite* circle;

		int startPosX;
		int startPosY;
		long double startRot;

		long double velocity;
		long double rotationalVelocity;

		int xRotOffset;
		int yRotOffset;
		long double rotationalOffset;
};
GravityWell_moving::GravityWell_moving(long double magnitude, long double edgeMagnitude, long double radius, Renderer* renderer, std::string texturePath, std::string circleTexturePath, long double texScale, long double circleTexScale, int posX, int posY, long double rotation, long double velocity, long double rotationalVelocity, long double rotationalOffset = 0, int xRotOffset = 0, int yRotOffset = 0)
	: GravityWell(magnitude, edgeMagnitude, radius)
{
	object = new Object(renderer);
	object->loadTexture(texturePath);
	object->scaleImage(texScale);

	circle = new Sprite(renderer);
	circle->loadTexture(circleTexturePath);
	circle->scaleImage(circleTexScale * radius);

	this->startPosX = posX;
	this->startPosY = posY;
	this->startRot = rotation;
	object->setPos(posX, posY);
	object->setRot(rotation);
	circle->setPos(posX, posY);
	circle->setRot(rotation);

	this->velocity = velocity;
	this->rotationalVelocity = rotationalVelocity;
	object->setVel(velocity);
	object->setVelR(rotationalVelocity);

	this->rotationalOffset = rotationalOffset;
	this->xRotOffset = xRotOffset;
	this->yRotOffset = yRotOffset;
}
void GravityWell_moving::render(int windowOffsetX = 0, int windowOffsetY = 0, long double rotationOffset = 0) {
	circle->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
	object->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
}
void GravityWell_moving::step(int windowWidth, int windowHeight, long double deltaT = 1.0) {
	object->physicsStep(deltaT);
	circle->setPos(object->getPosX(), object->getPosY());
	//std::cout << object->getVel() << std::endl;
}


class LevelController {
	public:
		LevelController();

		LTimer gameTime;
		LTimer deathTime;

		bool stop;
		bool pause;
		LTimer dtTimer;
		float timeSpeed;
};
LevelController::LevelController() {
	gameTime.start();

	stop = false;
	pause = false;
	timeSpeed = 1;
}

#endif