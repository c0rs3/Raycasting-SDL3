#ifndef	RAYCASTER_RENDERER
#define RAYCASTER_RENDERER

#include "raycasterutil.h"
#include "map.h"
#include "texture.h"

#define SCREENWIDTH 1720
#define SCREENHEIGHT 1080
#define mapWidth 24
#define mapHeight 24
#define texWidth 64
#define texHeight 64
#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888

class Camera {
public:
	Camera(double posX = 0, double posY = 0, double dirX = 0,
		double dirY = 0, double planeX = 0, double planeY = 0.66f, double moveSpeed = 0.1f, double rotSpeed = 0.1f)
		: posX(posX), posY(posY), dirX(dirX),
		dirY(dirY), planeX(planeX), planeY(planeY), moveSpeed(moveSpeed), rotSpeed(rotSpeed) {};
	~Camera() {};

	double posX, posY;
	double dirX, dirY;
	double planeX, planeY; // Vector of the camera plane
	double moveSpeed, rotSpeed;
private:
};

class Renderer {
public:
	Renderer(const Camera& camera, unsigned int screenWidth, unsigned int screenHeight)
		:mCamera(camera), mWindow_context(nullptr), mRender_context(nullptr), screenWidth(screenWidth), screenHeight(screenHeight) {}

	~Renderer();

	int render(Map& map);

private:
	Camera mCamera;
	SDL_Window* mWindow_context;
	SDL_Renderer* mRender_context;
	unsigned int screenWidth, screenHeight;
	uint32_t* buffer;
};
#endif