#ifndef RENDERER_
#define RENDERER_
#include <cstdlib>
#include <iostream>
#include <ostream>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "map.hpp"

#define PI 3.14159f

struct Keys {
	Keys() = default;

	int w = 0;
	int a = 0;
	int s = 0;
	int d = 0;
};

class Camera {
public:
	Camera(double posX = 0, double posY = 0, double dirX = 0,
		double dirY = 0, double planeX = 0, double planeY = 0.66f, double moveSpeed = 0.1f, double rotSpeed = 0.1f)
		: posX(posX), posY(posY), dirX(dirX),
		dirY(dirY), planeX(planeX), planeY(planeY), moveSpeed(moveSpeed), rotSpeed(rotSpeed) {
	};
	~Camera() {
	};

	double posX, posY;
	double dirX, dirY;
	double planeX, planeY; // Vector of the camera plane
	double moveSpeed, rotSpeed;
private:
};

class Renderer {
public:
	Renderer(const Camera& camera, unsigned int screenWidth, unsigned int screenHeight)
		:mCamera(camera), screenWidth(screenWidth), screenHeight(screenHeight) {
	}

	int render(Map& map) {
		double time = 0, oldTime = 0;
		double sRotSpeed;
		double cRotSpeed;

		double neg_sRotSpeed;
		double neg_cRotSpeed;

		if (!SDL_Init(SDL_INIT_VIDEO)) {
			std::clog << "Failed to init SDL video" << std::endl;
			return -1;
		}

		if (!SDL_CreateWindowAndRenderer("Ray-casting", screenWidth, screenHeight, 0, &mWindow_context, &mRender_context)) {
			std::clog << "Failed to init Window and/or Renderer" << std::endl;
			return -1;
		}

		bool quit = false;
		while (!quit) {
			SDL_RenderClear(mRender_context);
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

				// Calculate lowest and highest pixel to fill in current stripe
				int drawStart = -lineHeight / 2 + h / 2;
				if (drawStart < 0) drawStart = 0;
				int drawEnd = lineHeight / 2 + h / 2;
				if (drawEnd >= h) drawEnd = h - 1;

				switch (map.mData[mapX][mapY]) {
				case 1:
					// Set wall color
					if (side) {
						SDL_SetRenderDrawColor(mRender_context, 255, 255, 0, 255);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					else {
						SDL_SetRenderDrawColor(mRender_context, 120, 0, 255, 100);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					break;
				case 2:
					if (side) {
						SDL_SetRenderDrawColor(mRender_context, 255, 0, 0, 255);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					else {
						SDL_SetRenderDrawColor(mRender_context, 0, 0, 255, 100);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					break;
				case 3:
					if (side) {
						SDL_SetRenderDrawColor(mRender_context, 0, 255, 0, 255);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					else {
						SDL_SetRenderDrawColor(mRender_context, 100, 0, 255, 100);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					break;
				case 4:
					if (side) {
						SDL_SetRenderDrawColor(mRender_context, 255, 255, 255, 255);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					else {
						SDL_SetRenderDrawColor(mRender_context, 0, 0, 0, 100);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					break;
				default:
					if (side) {
						SDL_SetRenderDrawColor(mRender_context, 255, 255, 0, 255);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					else {
						SDL_SetRenderDrawColor(mRender_context, 120, 0, 255, 100);
						SDL_RenderLine(mRender_context, x, drawStart, x, drawEnd);
					}
					break;
				}


				// Set ceiling color
				SDL_SetRenderDrawColor(mRender_context, 30, 30, 30, 255);
				SDL_RenderLine(mRender_context, x, 0, x, drawStart);

				// Set floor color
				SDL_SetRenderDrawColor(mRender_context, 50, 50, 50, 255);
				SDL_RenderLine(mRender_context, x, drawEnd, x, screenHeight);
				SDL_SetRenderVSync(mRender_context, 1);
			}

			oldTime = time;
			time = SDL_GetTicks();
			double frameTime = (time - oldTime) / 1000.0;
			double fps = 1.0f / frameTime;
			if (int(SDL_GetTicks() / 1000) == 1)
				std::clog << "\r" << ((1.0 / frameTime) > 100.0f ? 100.0f : (1.0 / frameTime)) << ((1.0 / frameTime) > 100.0f ? "(>=100)" : "") << std::flush;

			if (frameTime < 0.001f) frameTime = 0.001f;
			if (frameTime > 0.05f)  frameTime = 0.05f;

			sRotSpeed = sin(mCamera.rotSpeed * frameTime * 5);
			cRotSpeed = cos(mCamera.rotSpeed * frameTime * 5);

			neg_sRotSpeed = sin(-mCamera.rotSpeed * frameTime * 5);
			neg_cRotSpeed = cos(-mCamera.rotSpeed * frameTime * 5);

			SDL_Event event;
			SDL_PollEvent(&event);
			// User requests quit
			if (event.type == SDL_EVENT_QUIT || event.key.scancode == SDL_SCANCODE_ESCAPE) {
				quit = true;
				SDL_DestroyWindow(mWindow_context);
				SDL_DestroyRenderer(mRender_context);
			}
			// move forward if no wall in front of you
			if (event.key.scancode == SDL_SCANCODE_UP) {
				if (!map.mData[int(mCamera.posX + mCamera.dirX * mCamera.moveSpeed)][int(mCamera.posY)]) mCamera.posX \
					+= mCamera.dirX * mCamera.moveSpeed;
				if (!map.mData[int(mCamera.posX)][int(mCamera.posY + mCamera.dirY * mCamera.moveSpeed)]) mCamera.posY \
					+= mCamera.dirY * mCamera.moveSpeed;
			}
			// move backwards if no wall behind you
			if (event.key.scancode == SDL_SCANCODE_DOWN) {
				if (!map.mData[int(mCamera.posX - mCamera.dirX * mCamera.moveSpeed)][int(mCamera.posY)]) mCamera.posX \
					-= mCamera.dirX * mCamera.moveSpeed;
				if (!map.mData[int(mCamera.posX)][int(mCamera.posY - mCamera.dirY * mCamera.moveSpeed)]) mCamera.posY \
					-= mCamera.dirY * mCamera.moveSpeed;
			}

			if (event.key.scancode == SDL_SCANCODE_RIGHT) {
				// both mCamera direction and mCamera plane must be rotated
				double oldDirX = mCamera.dirX;
				mCamera.dirX = mCamera.dirX * neg_cRotSpeed - mCamera.dirY * neg_sRotSpeed;
				mCamera.dirY = oldDirX * neg_sRotSpeed + mCamera.dirY * neg_cRotSpeed;
				double oldPlaneX = mCamera.planeX;
				mCamera.planeX = mCamera.planeX * neg_cRotSpeed - mCamera.planeY * neg_sRotSpeed;
				mCamera.planeY = oldPlaneX * neg_sRotSpeed + mCamera.planeY * neg_cRotSpeed;
			}
			if (event.key.scancode == SDL_SCANCODE_LEFT) {
				// both mCamera direction and mCamera plane must be rotated
				double oldDirX = mCamera.dirX;
				mCamera.dirX = mCamera.dirX * cRotSpeed - mCamera.dirY * sRotSpeed;
				mCamera.dirY = oldDirX * sRotSpeed + mCamera.dirY * cRotSpeed;
				double oldPlaneX = mCamera.planeX;
				mCamera.planeX = mCamera.planeX * cRotSpeed - mCamera.planeY * sRotSpeed;
				mCamera.planeY = oldPlaneX * sRotSpeed + mCamera.planeY * cRotSpeed;
			}
			SDL_RenderPresent(mRender_context);
		}
		return 0;
	}

private:
	Camera mCamera;
	SDL_Window* mWindow_context;
	SDL_Renderer* mRender_context;
	unsigned int screenWidth, screenHeight;
};
#endif