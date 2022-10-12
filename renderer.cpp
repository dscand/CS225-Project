#include <iostream>
#include "include/SDL.h"
#include "include/SDL_image.h"

class Renderer {
	private:
		SDL_Window* win;
		SDL_Renderer* rend;
	protected:
		int windowWidth;
		int windowHeight;
	public:
		Renderer(int windowWidth, int windowHeight) { this->windowWidth = windowWidth; this->windowHeight = windowHeight; }
		bool init();
		void close();
		SDL_Texture* loadTexture(SDL_Surface*& textureSurf) { return SDL_CreateTextureFromSurface(rend, textureSurf); }
		void clear();
		void update();
		void renderRTexture(SDL_Texture*& texture, SDL_Rect* srcrect, SDL_Rect* dstrect, double angle, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) { SDL_RenderCopyEx(rend, texture, srcrect, dstrect, angle, center, flip); }
};
bool Renderer::init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		success = false;
		std::printf("error initializing SDL: %s\n", SDL_GetError());
	}
	win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);

	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	return success;
}
void Renderer::close() {
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);

	rend = NULL;
	win = NULL;
		
	SDL_Quit();
}
void Renderer::clear() {
	SDL_RenderClear(rend);
}
void Renderer::update() {
	SDL_RenderPresent(rend);
}

class RTexture {
	public:
		RTexture(Renderer*);
		~RTexture() { free(); };

		bool loadTexture(std::string);
		void free();

		void setColor(Uint8 red, Uint8 green, Uint8 blue) { SDL_SetTextureColorMod(texture, red, green, blue); }
		void setBlendMode(SDL_BlendMode blending) { SDL_SetTextureBlendMode(texture, blending); }
		void setAlpha(Uint8 alpha) { SDL_SetTextureAlphaMod(texture, alpha); }

		void scaleImage(float scale) { width *= scale; height *= scale; }
		
		virtual void render(int x, int y, double angle, SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip);

		int getWidth() { return width; }
		int getHeight() { return height; }
		


	private:
		Renderer* renderer;
		SDL_Texture* texture;

		//Image dimensions
		int width;
		int height;
};
RTexture::RTexture(Renderer* renderer) {
	this->renderer = renderer;
	texture = NULL;

	width = 0;
	height = 0;
}
bool RTexture::loadTexture(std::string path) {
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));

	this->texture = renderer->loadTexture(loadedSurface);
	
	this->width = loadedSurface->w;
	this->height = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);

	return texture != NULL;
}
void RTexture::free() {
	//Free texture if it exists
	if ( texture != NULL ) {
		SDL_DestroyTexture( texture );
		texture = NULL;
		width = 0;
		height = 0;
	}
}
void RTexture::render(int x, int y, double angle = 0.0, SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	if( clip != NULL ) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	renderer->renderRTexture(texture, clip, &renderQuad, angle, center, flip);
}

class Sprite : public RTexture {
	public:
		Sprite(Renderer*);
		void setPosX(double x) { posX = x; }
		void setPosY(double y) { posY = y; }
		void setPos(double x, double y) { setPosX(x); setPosY(y); }
		double getPosX() { return posX; }
		double getPosY() { return posY; }
		void moveX(double magnitude) { posX += magnitude; }
		void moveY(double magnitude) { posY += magnitude; }
		void moveForward(double);
		void rotate(double);
		void setRot(double rotation) { this->rotation = rotation; }
		double getRot() { return rotation; }
		void render();

	private:
		double posX;
		double posY;
		double rotation;
};
Sprite::Sprite(Renderer* renderer)
	: RTexture(renderer)
{
	posX = 0.0;
	posY = 0.0;
	rotation = 0.0;
}
void Sprite::render() {
	RTexture::render(posX, posY, rotation);
}
void Sprite::moveForward(double magnitude) {
	moveX(-sin(-rotation*M_PI/180.0) * magnitude);
	moveY(-cos(-rotation*M_PI/180.0) * magnitude);
}
void Sprite::rotate(double magnitude) {
	rotation += magnitude;
	
	if (rotation >= 360.0) {
		int i = rotation / 360.0;
		rotation -= 360.0 * i;
	}
	if (rotation < 0) {
		int i = rotation / 360.0;
		rotation = (360.0 * (-i + 1)) + rotation;
	}
}

class Character : public Sprite {
	public:
		Character(Renderer*, double);
		void physicsStep(double);
		void addVel(double);
		void addVelX(double magnitude) { this->velX += magnitude; };
		void addVelY(double magnitude) { this->velY += magnitude; };
		void setVelX(double velX) { this->velX = velX; };
		void setVelY(double velY) { this->velY = velY; };
		double getVel() { return sqrt(velX*velX + velY*velY); };

	private:
		double velCap;
		double velX;
		double velY;
};
Character::Character(Renderer* renderer, double velCap)
	: Sprite(renderer)
{
	this->velCap = velCap;
	velX = 0.0;
	velY = 0.0;
}
void Character::physicsStep(double deltaT = 1.0) {
	/*{
	double vel = getVel();
	if (vel > velCap) {
		double multiplier = vel / velCap;
		velX /= multiplier;
		velY /= multiplier;
	}
	}*/

	/*{
	if (velX > velCap) {
		velX = velCap;
	}
	else if (velX < -velCap) {
		velX = -velCap;
	}

	if (velY > velCap) {
		velY = velCap;
	}
	else if (velY < -velCap) {
		velY = -velCap;
	}
	}*/

	moveX(velX * deltaT);
	moveY(velY * deltaT);
}
void Character::addVel(double magnitude) {
	double xVel = -sin(-getRot()*M_PI/180.0) * magnitude;
	double yVel = -cos(-getRot()*M_PI/180.0) * magnitude;

	addVelX(xVel);
	addVelY(yVel);
}