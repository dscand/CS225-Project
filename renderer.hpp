#ifndef renderer_hpp
#define renderer_hpp

#include <iostream>
#include <vector>

#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_mixer.h"

#include "helpers.hpp"

class Renderer {
	private:
		SDL_Window* win;
		SDL_Renderer* rend;
	protected:
		int windowWidth;
		int windowHeight;
	public:
		Renderer(int, int);
		~Renderer() { close(); }
		int getWindowWidth() { return windowWidth; }
		int getWindowHeight() { return windowHeight; }
		void setWindow(int width, int height) { windowWidth = width; windowHeight = height; }
		void init();
		void close();
		SDL_Texture* loadTexture(SDL_Surface* textureSurf) { return SDL_CreateTextureFromSurface(rend, textureSurf); }
		void clear();
		void update();
		void renderRTexture(SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect, long double angle, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) { SDL_RenderCopyEx(rend, texture, srcrect, dstrect, angle, center, flip); }
		int centerXCal(int, int, int);
		int centerYCal(int, int, int);
		Mix_Music* gMusic;
		std::vector<Mix_Chunk*> gSound;
};

class RTexture {
	public:
		RTexture(Renderer*);
		virtual ~RTexture() { free(); };

		bool loadTexture(std::string);
		void free();

		void setColor(Uint8 red, Uint8 green, Uint8 blue) { SDL_SetTextureColorMod(texture, red, green, blue); }
		void setBlendMode(SDL_BlendMode blending) { SDL_SetTextureBlendMode(texture, blending); }
		void setAlpha(Uint8 alpha) { SDL_SetTextureAlphaMod(texture, alpha); }

		void scaleImage(float scale) { width = _width * scale; height = _height * scale; }
		
		virtual void render(int x, int y, long double angle = 0.0, SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		int getWidth() { return width; }
		int getHeight() { return height; }

	private:
		Renderer* renderer;
		SDL_Texture* texture;

		//Image dimensions
		int _width;
		int _height;
		int width;
		int height;
};

class Sprite : public RTexture {
	public:
		Sprite(Renderer*);
		void setPosX(long double x) { posX = x; }
		void setPosY(long double y) { posY = y; }
		void setPos(long double x, long double y) { setPosX(x); setPosY(y); }
		long double getPosX() { return posX; }
		long double getPosY() { return posY; }
		void moveX(long double magnitude) { posX += magnitude; }
		void moveY(long double magnitude) { posY += magnitude; }
		void moveForward(long double);
		void rotate(long double);
		void setRot(long double);
		long double getRot() { return rotation; }
		void render(int, int, long double);

	private:
		long double posX;
		long double posY;
		long double rotation;
};

class GravityWell {
	public:
		GravityWell(long double magnitude, long double radius) { this->magnitude = magnitude; this->radius = radius; }
		virtual ~GravityWell() {};
		virtual long double calcGravityMag(int posX, int posY) { std::cout << "ERROR: Undefined, calcGravityMag" << std::endl; return 0; }
		virtual long double calcGravityRot(int posX, int posY) { std::cout << "ERROR: Undefined, calcGravityRot" << std::endl; return 0; }
		virtual void render(int, int, long double) {}

	protected:
		long double calcGravityWellMag(int, int, int, int);
		long double calcGravityWellRot(int, int, int, int);

	private:
		long double magnitude;
		long double radius;
};

class Object : public Sprite {
	public:
		Object(Renderer*);
		void physicsStep(long double);
		bool boxCollision(int, int, int, int);
		bool circleCollision(int, int, long double);
		void addVel(long double);
		void addVelX(long double magnitude) { this->velX += magnitude; }
		void addVelY(long double magnitude) { this->velY += magnitude; }
		void setVel(long double);
		void setVel(long double velX, long double velY) { setVelX(velX); setVelY(velY); }
		void setVelX(long double velX) { this->velX = velX; }
		void setVelY(long double velY) { this->velY = velY; }
		void setVelR(long double velR) { this->velR = velR; }
		long double getVel() { return sqrt(velX*velX + velY*velY); }
		long double getVelX() { return velX; }
		long double getVelY() { return velY; }
		long double getVelR() { return velR; }
		void addInfluence(GravityWell* influence) { influences.push_back(influence); }

	private:
		std::vector<GravityWell*> influences;
		long double velX;
		long double velY;
		long double velR;
};

#endif
