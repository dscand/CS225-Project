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
};
Renderer::Renderer(int windowWidth, int windowHeight) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	win = nullptr;
	rend = nullptr;
}
void Renderer::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::printf("error initializing SDL: %s\n", SDL_GetError());
	}
	win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
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
	//SDL_GetWindowSize(win, &windowWidth, &windowHeight);
}
void Renderer::update() {
	SDL_RenderPresent(rend);
}
int Renderer::centerXCal(int posX, int offset, int max) {
	int centerX = 0;

	if (posX + offset > windowWidth) {
		centerX = windowWidth - posX - offset;
	}

	if (-centerX > max) {
		centerX = -max;
	}
	return centerX;
}
int Renderer::centerYCal(int posY, int offset, int max) {
	int centerY = 0;

	if (posY + offset > windowHeight) {
		centerY = windowHeight - posY - offset;
	}

	if (-centerY > max) {
		centerY = -max;
	}
	return centerY;
}

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
		
		virtual void render(int x, int y, long double angle, SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip);

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
RTexture::RTexture(Renderer* renderer) {
	this->renderer = renderer;
	texture = NULL;

	_width = 0;
	_height = 0;
	width = 0;
	height = 0;
}
bool RTexture::loadTexture(std::string path) {
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));

	this->texture = renderer->loadTexture(loadedSurface);
	
	this->_width = loadedSurface->w;
	this->_height = loadedSurface->h;
	this->width = this->_width;
	this->height = this->_height;

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
void RTexture::render(int x, int y, long double angle = 0.0, SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	renderer->renderRTexture(texture, clip, &renderQuad, angle, center, flip);
}

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
Sprite::Sprite(Renderer* renderer)
	: RTexture(renderer)
{
	posX = 0.0;
	posY = 0.0;
	rotation = 0.0;
}
void Sprite::render(int windowOffsetX = 0, int windowOffsetY = 0, long double rotationOffset = 0) {
	RTexture::render(posX + windowOffsetX - (getWidth()/2), posY + windowOffsetY - (getHeight()/2), rotation + rotationOffset);
}
void Sprite::moveForward(long double magnitude) {
	moveX(sin(degrees2radians(rotation)) * magnitude);
	moveY(-cos(degrees2radians(rotation)) * magnitude);
}
void Sprite::rotate(long double magnitude) {
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
void Sprite::setRot(long double rotation) {
	if (rotation >= 360.0) {
		int i = rotation / 360.0;
		rotation -= 360.0 * i;
	}
	if (rotation < 0) {
		int i = rotation / 360.0;
		rotation = (360.0 * (-i + 1)) + rotation;
	}

	this->rotation = rotation;
}

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
long double GravityWell::calcGravityWellMag(int pos1X, int pos1Y, int pos2X, int pos2Y) {
	long double distance = calcDistance(pos1X, pos1Y, pos2X, pos2Y);
	if (distance < radius) {
		distance /= 1000;
		//std::cout << magnitude/(distance*distance) << std::endl;
		return magnitude/(distance*distance);
	}
	else {
		return 0;
	}
}
long double GravityWell::calcGravityWellRot(int pos1X, int pos1Y, int pos2X, int pos2Y) {
	return calcDirection(pos1X, pos1Y, pos2X, pos2Y);
}

class Object : public Sprite {
	public:
		Object(Renderer*);
		void physicsStep(long double);
		void addVel(long double);
		void addVelX(long double magnitude) { this->velX += magnitude; }
		void addVelY(long double magnitude) { this->velY += magnitude; }
		void setVel(long double);
		void setVelX(long double velX) { this->velX = velX; }
		void setVelY(long double velY) { this->velY = velY; }
		void setVelR(long double velR) { this->velR = velR; }
		long double getVel() { return sqrt(velX*velX + velY*velY); }
		long double getVelX() { return velX; }
		long double getVelY() { return velY; }
		long double getVelR() { return velR; }
		void addInfluence(GravityWell* influence) { influences.push_back(influence); };
		//void removeInfluence(GravityWell*); // TODO

	private:
		std::vector<GravityWell*> influences;
		long double velX;
		long double velY;
		long double velR;
};
Object::Object(Renderer* renderer)
	: Sprite(renderer)
{
	influences = {};
	
	velX = 0.0;
	velY = 0.0;
	velR = 0.0;
}
void Object::physicsStep(long double deltaT = 1.0) {
	//Influences
	for (GravityWell* influence : influences) {
		long double magnitude = influence->calcGravityMag(getPosX(), getPosY());

		if (magnitude > 0) {
			long double direction = influence->calcGravityRot(getPosX(), getPosY());

			velX += sin(degrees2radians(direction)) * magnitude * deltaT;
			velY += -cos(degrees2radians(direction)) * magnitude * deltaT;
		}
	}

	const int max = 1000;
	if (velX > max) velX = max;
	else if (velX < -max) velX = -max;
	if (velY > max) velY = max;
	else if (velY < -max) velY = -max;

	moveX(velX * deltaT);
	moveY(velY * deltaT);

	if (velR != 0.0) {
		rotate(velR * deltaT);
	}
}
void Object::addVel(long double magnitude) {
	long double xVel = sin(degrees2radians(getRot())) * magnitude;
	long double yVel = -cos(degrees2radians(getRot())) * magnitude;

	addVelX(xVel);
	addVelY(yVel);
}
void Object::setVel(long double magnitude) {
	long double xVel = sin(degrees2radians(getRot())) * magnitude;
	long double yVel = -cos(degrees2radians(getRot())) * magnitude;
	
	setVelX(xVel);
	setVelY(yVel);
}