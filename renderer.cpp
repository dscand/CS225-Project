#include "renderer.hpp"

Renderer::Renderer(int windowWidth, int windowHeight) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	win = nullptr;
	rend = nullptr;
}
void music_init(Renderer* renderer){
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	
	renderer->gMusic = Mix_LoadMUS( "Sounds/terminal_theme.wav" );
	
	//renderer->gSound.push_back(Mix_LoadWAV("fileLocation/fileName.wav"));
	
}
void Renderer::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::printf("error initializing SDL: %s\n", SDL_GetError());
	}
	win = SDL_CreateWindow("Terminal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_BORDERLESS);
	
	const std::string iconPath = "Textures/Icon.png";
	SDL_Surface* icon = IMG_Load(iconPath.c_str()); // 16x16
	SDL_SetWindowIcon(win, icon);

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
void RTexture::render(int x, int y, long double angle, SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	renderer->renderRTexture(texture, clip, &renderQuad, angle, center, flip);
}

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

long double GravityWell::calcGravityWellMag(int pos1X, int pos1Y, int pos2X, int pos2Y) {
	long double distance = calcDistance(pos1X, pos1Y, pos2X, pos2Y);
	if (distance < radius) {
		distance /= 1000;
		return magnitude/(distance*distance);
	}
	else {
		return 0;
	}
}
long double GravityWell::calcGravityWellRot(int pos1X, int pos1Y, int pos2X, int pos2Y) {
	return calcDirection(pos1X, pos1Y, pos2X, pos2Y);
}

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
		long double direction = influence->calcGravityRot(getPosX(), getPosY());

		velX += sin(degrees2radians(direction)) * magnitude * deltaT;
		velY += -cos(degrees2radians(direction)) * magnitude * deltaT;
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
bool Object::boxCollision(int x1, int y1, int x2, int y2) {
	// Top Left and Bottom Right Corner
	bool hit = false;

	// right boundary
	if (getPosX() > x2) {
		setPosX((2 * x2) - getPosX());
		setVelX(-getVelX());
		hit = true;
	}

	// left boundary
	if (getPosX() < x1) {
		setPosX((2 * x1) - getPosX());
		setVelX(-getVelX());
		hit = true;
	}

	// bottom boundary
	if (getPosY() > y2) {
		setPosY((2 * y2) - getPosY());
		setVelY(-getVelY());
		hit = true;
	}

	// upper boundary
	if (getPosY() < y1) {
		setPosY((2 * y1) - getPosY());
		setVelY(-getVelY());
		hit = true;
	}

	return hit;
}
bool Object::circleCollision(int x, int y, long double r) {
	// Top Left and Bottom Right Corner
	bool hit = false;

	long double distance = calcDistance(getPosX(), getPosY(), x, y);
	if (distance <= r) {
		// TODO
		//long double direction = calcDirection(getPosX(), getPosY(), x, y);
		//long double parallelVel = cos(degrees2radians(direction)) * getVel();
		//std::cout << direction << std::endl;
		hit = true;
	}

	return hit;
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
