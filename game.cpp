#include "game.hpp"

Background::Background(Renderer* renderer, std::string texturePath, long double imageScale = 1) {
	texture = new RTexture(renderer);
	texture->loadTexture(texturePath);
	texture->scaleImage(imageScale);
}
void Background::render(int windowOffsetX, int windowOffsetY) {
	texture->render(windowOffsetX, windowOffsetY);
}


Texture::Texture(Renderer* renderer, std::string texturePath, long double imageScale, int posX, int posY, long double rotation) {
	draw = true;
	texture = new Sprite(renderer);
	texture->loadTexture(texturePath);
	texture->scaleImage(imageScale);

	this->posX = posX;
	this->posY = posY;
	this->rotation = rotation;
}
void Texture::render(int windowOffsetX, int windowOffsetY) {
	if (draw) texture->render(posX + windowOffsetX, posY + windowOffsetY, rotation);
}


GravityWell_stationary::GravityWell_stationary(long double magnitude, long double radius, long double collisionRadius, Renderer* renderer, std::string texturePath, std::string circleTexturePath, long double texScale, long double circleTexScale, int posX, int posY, long double rotation, long double rotationalOffset = 0, int xRotOffset = 0, int yRotOffset = 0)
	: GravityWell(magnitude, radius)
{
	sprite = new Sprite(renderer);
	sprite->loadTexture(texturePath);
	sprite->scaleImage(texScale);

	circle = new Sprite(renderer);
	circle->loadTexture(circleTexturePath);
	circle->scaleImage(circleTexScale * radius);

	this->collisionRadius = collisionRadius;

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
void GravityWell_stationary::render(int windowOffsetX, int windowOffsetY, long double rotationOffset) {
	circle->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
	sprite->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
}


GravityWell_moving::GravityWell_moving(long double magnitude, long double radius, long double collisionRadius, Renderer* renderer, std::string texturePath, std::string circleTexturePath, long double texScale, long double circleTexScale, int posX, int posY, long double rotation, long double velocity, long double rotationalVelocity, long double rotationalOffset = 0, int xRotOffset = 0, int yRotOffset = 0)
	: GravityWell(magnitude, radius)
{
	object = new Object(renderer);
	object->loadTexture(texturePath);
	object->scaleImage(texScale);

	circle = new Sprite(renderer);
	circle->loadTexture(circleTexturePath);
	circle->scaleImage(circleTexScale * radius);

	this->collisionRadius = collisionRadius;

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
void GravityWell_moving::render(int windowOffsetX, int windowOffsetY, long double rotationOffset) {
	circle->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
	object->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
}
void GravityWell_moving::step(long double deltaT) {
	object->physicsStep(deltaT);
	circle->setPos(object->getPosX(), object->getPosY());
	circle->setRot(object->getRot());
	//std::cout << object->getVel() << std::endl;
}


StarCoin::StarCoin(Renderer* renderer, std::vector<std::string> texturePaths, long double texScale, int posX, int posY, long double rotation, long double rotationalOffset = 0, int xRotOffset = 0, int yRotOffset = 0) {
	active = true;
	
	for (std::string path : texturePaths) {
		Sprite* _sprite = new Sprite(renderer);
		_sprite->loadTexture(path);
		_sprite->scaleImage(texScale);
		_sprite->setPos(posX, posY);
		_sprite->setRot(rotation);
		sprites.push_back(_sprite);
	}
	spriteSel = 0;

	this->posX = posX;
	this->posY = posY;
	this->rotation = rotation;

	this->rotationalOffset = rotationalOffset;
	this->xRotOffset = xRotOffset;
	this->yRotOffset = yRotOffset;
}
void StarCoin::render(int windowOffsetX, int windowOffsetY, long double rotationOffset) {
	if (active) {
		sprites.at(spriteSel)->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
		spriteSel = rand() % sprites.size();
	}
}


Player::Player(Renderer* renderer, std::string shipTexturePath, std::string shipBrokenTexturePath, long double shipScale, std::vector<std::string> flameTexturePath, long double flameScale, std::vector<std::string> explosion1TexturePath, std::vector<std::string> explosion2TexturePath, std::vector<std::string> explosion3TexturePath, long double explosionScale, long double speed = 10, long double rotationSpeed = 180, long double rotationalOffset = 0, int xRotOffset = 0, int yRotOffset = 0) {
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
		spriteFlame.push_back(_sprite);
	}

	for (std::string path : explosion1TexturePath) {
		Sprite* _sprite = new Sprite(renderer);
		_sprite->loadTexture(path);
		_sprite->scaleImage(explosionScale);
		spriteExplosion1.push_back(_sprite);
	}
	for (std::string path : explosion2TexturePath) {
		Sprite* _sprite = new Sprite(renderer);
		_sprite->loadTexture(path);
		_sprite->scaleImage(explosionScale);
		spriteExplosion2.push_back(_sprite);
	}
	for (std::string path : explosion3TexturePath) {
		Sprite* _sprite = new Sprite(renderer);
		_sprite->loadTexture(path);
		_sprite->scaleImage(explosionScale);
		spriteExplosion3.push_back(_sprite);
	}

	alive = true;
	boosting = false;
	flameSel = 0;
	explosionSel = 0;
	explosionIndex = 0;

	this->speed = speed;
	this->rotationSpeed = rotationSpeed;
	this->velocityCap = velocityCap;

	this->rotationalOffset = rotationalOffset;
	this->xRotOffset = xRotOffset;
	this->yRotOffset = yRotOffset;
}
void Player::render(int windowOffsetX, int windowOffsetY, long double rotationOffset) {
	if (alive) object->render(windowOffsetX, windowOffsetY, rotationOffset + rotationalOffset);
	else if (explosionIndex == 0) {
		spriteBroken->render(windowOffsetX, windowOffsetY, object->getRot() + rotationalOffset);
	}

	if (boosting && alive) {
		spriteFlame.at(flameSel)->render(windowOffsetX, windowOffsetY, object->getRot() + rotationalOffset);
		flameSel = rand() % spriteFlame.size();
	}
	else if (!alive) {
		if (explosionIndex == 1) {
			spriteExplosion1.at(explosionSel)->render(windowOffsetX, windowOffsetY, object->getRot() + rotationalOffset);
			explosionSel = rand() % spriteExplosion1.size();
		}
		else if (explosionIndex == 2) {
			spriteExplosion2.at(explosionSel)->render(windowOffsetX, windowOffsetY, object->getRot() + rotationalOffset);
			explosionSel = rand() % spriteExplosion2.size();
		}
		else if (explosionIndex == 3) {
			spriteExplosion3.at(explosionSel)->render(windowOffsetX, windowOffsetY, object->getRot() + rotationalOffset);
			explosionSel = rand() % spriteExplosion3.size();
		}
	}
	
}
void Player::boost(long double multiplier) {
	if (alive) object->addVel(speed * multiplier);
	boosting = true;

	if (!Mix_Playing(2)) Mix_PlayChannel(2, object->renderer->gSound[2], 0);
}
void Player::playerStep(long double deltaT = 1.0) {
	object->physicsStep(deltaT);
	spriteBroken->setPos(object->getPosX(), object->getPosY());
	spriteFlame.at(flameSel)->setPos(object->getPosX(), object->getPosY());

	if(!alive) {
		spriteExplosion1.at(explosionSel)->setPos(object->getPosX(), object->getPosY());
		spriteExplosion2.at(explosionSel)->setPos(object->getPosX(), object->getPosY());
		spriteExplosion3.at(explosionSel)->setPos(object->getPosX(), object->getPosY());
	}
}
void Player::wallCollision(int gameWidth, int gameHeight) {
	const int deathVelocity = 100;
	if (object->boxCollision(0,0, gameWidth,gameHeight) && abs(object->getVel()) > deathVelocity) {
		if (alive) Mix_PlayChannel(1, object->renderer->gSound[1], 0);
		alive = false;
	}
}
void Player::objectCollision(GravityWell_stationary* collisionObject) {
	const int deathVelocity = 100;
	if (object->circleCollision(collisionObject->getPosX(),collisionObject->getPosY(), collisionObject->collisionRadius) && abs(object->getVel()) > deathVelocity) {
		if (alive) Mix_PlayChannel(1, object->renderer->gSound[1], 0);
		alive = false;
	}
}
void Player::objectCollision(GravityWell_moving* collisionObject) {
	const int deathVelocity = 100;
	if (object->circleCollision(collisionObject->getPosX(),collisionObject->getPosY(), collisionObject->collisionRadius) && abs(object->getVel()) > deathVelocity) {
		if (alive) Mix_PlayChannel(1, object->renderer->gSound[1], 0);
		alive = false;
	}
}
