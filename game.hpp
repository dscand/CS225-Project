#ifndef game
#define game

#include <iostream>
#include <vector>

#include "renderer.hpp"

class Background {
	public:
		Background(Renderer*, std::string, long double);
		~Background() { delete texture; texture = nullptr; }
		void render(int = 0, int = 0);

	private:
		RTexture* texture;
};

class Texture {
	public:
		bool draw;
		Texture(Renderer*, std::string, long double, int, int, long double);
		~Texture() { delete texture; texture = nullptr; }
		void render(int = 0, int = 0);
		int getPosX() { return posX; }
		int getPosY() { return posY; }
		int getWidth() { return texture->getWidth(); }
		int getHeight() { return texture->getHeight(); }

	private:
		Sprite* texture;

		int posX;
		int posY;
		long double rotation;
};

class GravityWell_stationary : public GravityWell {
	public:
		GravityWell_stationary(long double, long double, long double, Renderer*, std::string, std::string, long double, long double, int, int, long double, long double, int, int);
		~GravityWell_stationary() { delete sprite; sprite = nullptr; delete circle; circle = nullptr; }
		void render(int = 0, int = 0, long double = 0);
		long double calcGravityMag(int posX, int posY) { return calcGravityWellMag(this->posX, this->posY, posX, posY); }
		long double calcGravityRot(int posX, int posY) { return calcGravityWellRot(this->posX, this->posY, posX, posY); }
		int getPosX() { return sprite->getPosX(); }
		int getPosY() { return sprite->getPosY(); }

		long double collisionRadius;

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
class GravityWell_moving : public GravityWell {
	public:
		GravityWell_moving(long double, long double, long double, Renderer*, std::string, std::string, long double, long double, int, int, long double, long double, long double, long double, int, int);
		~GravityWell_moving() { delete object; object = nullptr; delete circle; circle = nullptr; }
		void step(long double = 1.0);
		void render(int = 0, int = 0, long double = 0);
		long double calcGravityMag(int posX, int posY) { return calcGravityWellMag(object->getPosX(), object->getPosY(), posX, posY); }
		long double calcGravityRot(int posX, int posY) { return calcGravityWellRot(object->getPosX(), object->getPosY(), posX, posY); }
		void setVel(long double vel) { object->setVel(vel); }
		long double getVel() { return object->getVel(); }
		int getPosX() { return object->getPosX(); }
		int getPosY() { return object->getPosY(); }
		void addInfluence(GravityWell* influence) { object->addInfluence(influence); }

		long double collisionRadius;
	
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

class StarCoin {
	public:
		StarCoin(Renderer*, std::vector<std::string>, long double, int, int, long double, long double, int, int);
		~StarCoin() { sprites.clear(); }
		void render(int = 0, int = 0, long double = 0);
		bool active;
		int getPosX() { return posX; }
		int getPosY() { return posY; }

	private:
		std::vector<Sprite*> sprites;
		int spriteSel;

		int posX;
		int posY;
		long double rotation;

		int xRotOffset;
		int yRotOffset;
		long double rotationalOffset;
};

class Player {
	public:
		Player(Renderer*, std::string, std::string, long double, std::vector<std::string>, long double, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, long double, long double, long double, long double, int, int);
		~Player() { delete object; object = nullptr; spriteFlame.clear(); spriteExplosion1.clear(); spriteExplosion2.clear(); spriteExplosion3.clear(); }
		void playerStep(long double);
		void wallCollision(int, int);
		void objectCollision(GravityWell_stationary*);
		void objectCollision(GravityWell_moving*);
		void render(int = 0, int = 0, long double = 0);
		void boost(long double multiplier = 1);
		void nonboost() { boosting = false; Mix_HaltChannel(2); }
		void rotate(long double multiplier = 1) { if (alive) object->rotate(rotationSpeed * multiplier); }
		void setPosX(long double posX) { object->setPosX(posX); }
		void setPosY(long double posY) { object->setPosY(posY); }
		long double getPosX() { return object->getPosX(); }
		long double getPosY() { return object->getPosY(); }
		long double getOffsetX() { return object->getWidth() / 2; }
		long double getOffsetY() { return object->getHeight() / 2; }
		long double getVel() { return object->getVel(); }
		void setVel(long double velX, long double velY) { object->setVel(velX, velY); }
		void addInfluence(GravityWell* influence) { object->addInfluence(influence); }
		bool isAlive() { return alive; }
		int explosionIndex;

	private:
		Object* object;
		Sprite* spriteBroken;
		std::vector<Sprite*> spriteFlame;
		std::vector<Sprite*> spriteExplosion1;
		std::vector<Sprite*> spriteExplosion2;
		std::vector<Sprite*> spriteExplosion3;

		bool alive;
		bool boosting;
		int flameSel;
		int explosionSel;

		long double speed;
		long double rotationSpeed;
		long double velocityCap;

		int xRotOffset;
		int yRotOffset;
		long double rotationalOffset;
};

#endif