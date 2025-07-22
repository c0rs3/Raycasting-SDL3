#ifndef	RAYCASTER_RENDERER
#define RAYCASTER_RENDERER
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_keyboard.h"
#include "map.hpp"

#define SCREENWIDTH 1720
#define SCREENHEIGHT 1080
#define mapWidth 24
#define mapHeight 24
#define texWidth 64
#define texHeight 64

inline Uint32 makeRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}


int worldMap[mapWidth][mapHeight] =
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

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

	~Renderer() {
		delete[] buffer;
	}

	int render(Map& map) {
		buffer = new uint32_t[screenHeight * screenWidth];
		memset(buffer, 0, screenWidth * screenHeight * sizeof(uint32_t));

#define pixelFormat SDL_PIXELFORMAT_RGBA8888
		std::vector<int> texture[8];
		for (int i = 0; i < 8; i++) texture[i].resize(texWidth * texHeight);
		for (int x = 0; x < texWidth; x++) {
			for (int y = 0; y < texHeight; y++) {
				int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
				int ycolor = y * 256 / texHeight;
				int xycolor = y * 128 / texHeight + x * 128 / texWidth;

				// Proper color building with alpha = 255
				texture[0][texWidth * y + x] = makeRGBA(
					254 * (x != y && x != texWidth - y), 0, 0
				);
				texture[1][texWidth * y + x] = makeRGBA(
					xycolor, xycolor, xycolor
				);
				texture[2][texWidth * y + x] = makeRGBA(
					xycolor, xycolor, 0
				);
				texture[3][texWidth * y + x] = makeRGBA(
					xorcolor, xorcolor, xorcolor
				);
				texture[4][texWidth * y + x] = makeRGBA(
					0, xorcolor, 0
				);
				texture[5][texWidth * y + x] = makeRGBA(
					192 * (x % 16 && y % 16), 0, 0
				);
				texture[6][texWidth * y + x] = makeRGBA(
					ycolor, 0, 0
				);
				texture[7][texWidth * y + x] = makeRGBA(
					128, 128, 128
				);
			}
		}

		double time = 0, oldTime = 0;
		double sRotSpeed;
		double cRotSpeed;

		double neg_sRotSpeed;
		double neg_cRotSpeed;

		if (!SDL_Init(SDL_INIT_VIDEO)) {
			std::clog << "Failed to init SDL video: " << SDL_GetError() << std::endl;
			return -1;
		}

		if (!SDL_CreateWindowAndRenderer("Ray-casting", screenWidth, screenHeight, 0, &mWindow_context, &mRender_context)) {
			std::clog << "Failed to init Window and/or Renderer" << SDL_GetError() << std::endl;
			return -1;
		}

		SDL_Texture* render_texture = SDL_CreateTexture(
			mRender_context, pixelFormat, SDL_TEXTUREACCESS_STREAMING,
			screenWidth, screenHeight);

		bool quit = false;
		while (!quit) {
			SDL_Event event;
			SDL_PollEvent(&event);
			const bool* keyState = SDL_GetKeyboardState(nullptr);
			memset(buffer, 0, screenWidth * screenHeight * sizeof(uint32_t));

			for (unsigned int x = 0; x < screenWidth; ++x) {
				double mCameraX = 2 * x / double(screenWidth) - 1;
				double rayDirX = mCamera.dirX + mCamera.planeX * mCameraX;
				double rayDirY = mCamera.dirY + mCamera.planeY * mCameraX;

				// coordinates of the mCamera on the 2D array
				int mapX = int(mCamera.posX);
				int mapY = int(mCamera.posY);

				// length of ray from current mCamera position to next x or y-side
				double sideDistX;
				double sideDistY;

				// length of ray from one x or y-side to next x or y-side
				double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
				double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
				double perpWallDist;

				// what direction to step in x or y-direction (either +1 or -1)
				int stepX;
				int stepY;

				bool hit = 0;
				bool side; // NS or a EW wall

				if (rayDirX < 0) {
					stepX = -1;
					sideDistX = (mCamera.posX - mapX) * deltaDistX;
				}
				else {
					stepX = 1;
					sideDistX = (mapX + 1.0 - mCamera.posX) * deltaDistX;
				}
				if (rayDirY < 0) {
					stepY = -1;
					sideDistY = (mCamera.posY - mapY) * deltaDistY;
				}
				else {
					stepY = 1;
					sideDistY = (mapY + 1.0 - mCamera.posY) * deltaDistY;
				}
				// DDA
				while (hit == 0) {
					// jump to next map square, unless there is a hit
					if (sideDistX < sideDistY) {
						sideDistX += deltaDistX;
						mapX += stepX;
						side = 0;
					}
					else {
						sideDistY += deltaDistY;
						mapY += stepY;
						side = 1;
					}
					if (map.mData[mapX][mapY] > 0) hit = 1;
				}

				// Calculate distance projected on mCamera direction
				if (side == 0) perpWallDist = (sideDistX - deltaDistX);
				else          perpWallDist = (sideDistY - deltaDistY);

				unsigned int h = screenHeight;
				int lineHeight = (int)(h / perpWallDist);

				int drawStart = -lineHeight / 2 + h / 2;
				if (drawStart < 0) drawStart = 0;
				int drawEnd = lineHeight / 2 + h / 2;

				if (drawEnd >= h) drawEnd = h - 1;

				int texNum = worldMap[mapX][mapY] - 1; // 1 subtracted from it so that texture 0 can be used!

				//calculate value of wallX
				double wallX; //where exactly the wall was hit
				if (side == 0) wallX = mCamera.posY + perpWallDist * rayDirY;
				else           wallX = mCamera.posX + perpWallDist * rayDirX;
				wallX -= floor((wallX));

				//x coordinate on the texture
				int texX = int(wallX * double(texWidth));
				if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
				if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

				// How much to increase the texture coordinate per screen pixel
				double step = 1.0 * texHeight / lineHeight;
				// Starting texture coordinate
				double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
				for (int y = drawStart; y < drawEnd; y++) {
					// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
					int texY = (int)texPos & (texHeight - 1);
					texPos += step;
					Uint32 color = texture[texNum][texHeight * texY + texX];
					//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
					if (side == 1) color = (color >> 1) & 8355711;
					buffer[y * screenWidth + x] = color;
				}
			}
			SDL_RenderClear(mRender_context);
			if (SDL_UpdateTexture(render_texture, nullptr, buffer, screenWidth * sizeof(uint32_t)) < 0) {
				std::clog << "UpdateTexture failed: " << SDL_GetError() << std::endl;
			}

			if (SDL_RenderTexture(mRender_context, render_texture, nullptr, nullptr) < 0) {
				std::clog << "UpdateTexture failed: " << SDL_GetError() << std::endl;
			}

			oldTime = time;
			time = SDL_GetTicks();
			double frameTime = (time - oldTime) / 1000.0;
			double fps = 1.0f / frameTime;
			if (int(SDL_GetTicks() / 1000) == 1)
				std::clog << "\r" << (1.0 / frameTime) << std::flush;
			// std::clog << "\r" << ((1.0 / frameTime) > 100.0f ? ">=100.0" : std::to_string(1.0/frameTime))<< std::flush;
			if (frameTime < 0.001f) frameTime = 0.001f;
			if (frameTime > 0.05f)  frameTime = 0.05f;

			sRotSpeed = sin(mCamera.rotSpeed * frameTime * 5);
			cRotSpeed = cos(mCamera.rotSpeed * frameTime * 5);

			neg_sRotSpeed = sin(-mCamera.rotSpeed * frameTime * 5);
			neg_cRotSpeed = cos(-mCamera.rotSpeed * frameTime * 5);

			// User requests quit
			if (event.type == SDL_EVENT_QUIT || event.key.scancode == SDL_SCANCODE_ESCAPE) {
				quit = true;
				SDL_DestroyWindow(mWindow_context);
				SDL_DestroyRenderer(mRender_context);
			}

			if (keyState[SDL_SCANCODE_UP]) {
				if (!map.mData[int(mCamera.posX + mCamera.dirX * mCamera.moveSpeed)][int(mCamera.posY)]) mCamera.posX \
					+= mCamera.dirX * mCamera.moveSpeed;
				if (!map.mData[int(mCamera.posX)][int(mCamera.posY + mCamera.dirY * mCamera.moveSpeed)]) mCamera.posY \
					+= mCamera.dirY * mCamera.moveSpeed;
			}

			if (keyState[SDL_SCANCODE_DOWN]) {
				if (!map.mData[int(mCamera.posX - mCamera.dirX * mCamera.moveSpeed)][int(mCamera.posY)]) mCamera.posX \
					-= mCamera.dirX * mCamera.moveSpeed;
				if (!map.mData[int(mCamera.posX)][int(mCamera.posY - mCamera.dirY * mCamera.moveSpeed)]) mCamera.posY \
					-= mCamera.dirY * mCamera.moveSpeed;
			}

			if (keyState[SDL_SCANCODE_RIGHT]) {
				double oldDirX = mCamera.dirX;
				mCamera.dirX = mCamera.dirX * neg_cRotSpeed - mCamera.dirY * neg_sRotSpeed;
				mCamera.dirY = oldDirX * neg_sRotSpeed + mCamera.dirY * neg_cRotSpeed;
				double oldPlaneX = mCamera.planeX;
				mCamera.planeX = mCamera.planeX * neg_cRotSpeed - mCamera.planeY * neg_sRotSpeed;
				mCamera.planeY = oldPlaneX * neg_sRotSpeed + mCamera.planeY * neg_cRotSpeed;
			}
			if (keyState[SDL_SCANCODE_LEFT]) {
				double oldDirX = mCamera.dirX;
				mCamera.dirX = mCamera.dirX * cRotSpeed - mCamera.dirY * sRotSpeed;
				mCamera.dirY = oldDirX * sRotSpeed + mCamera.dirY * cRotSpeed;
				double oldPlaneX = mCamera.planeX;
				mCamera.planeX = mCamera.planeX * cRotSpeed - mCamera.planeY * sRotSpeed;
				mCamera.planeY = oldPlaneX * sRotSpeed + mCamera.planeY * cRotSpeed;

			}
			SDL_RenderPresent(mRender_context);
		}
		SDL_DestroyTexture(render_texture);
		return 0;
	}

private:
	Camera mCamera;
	SDL_Window* mWindow_context;
	SDL_Renderer* mRender_context;
	unsigned int screenWidth, screenHeight;
	uint32_t* buffer;
};
#endif