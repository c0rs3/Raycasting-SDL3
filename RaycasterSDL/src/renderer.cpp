#include <Raycaster.hpp>
#include <Raycaster/Camera.hpp>
#include <Raycaster/Map.hpp>
#include <Raycaster/Renderer.hpp>
#include <Raycaster/Texture.hpp>

#include <assert.h>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <thread>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#define TEXTURE_ASSET_PATH "assets/textures/"
#define DEFAULT_TEXTWIDTH 64
#define DEFAULT_TEXTHEIGHT 64

Renderer::Renderer(uint32_t width, uint32_t height) {
    mRendererWidth = width;
    mRendererHeight = height;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::clog << "Failed to init SDL video: " << SDL_GetError() << std::endl;
    }

    if (!SDL_CreateWindowAndRenderer("Game", mRendererWidth, mRendererHeight, 0,
        &mWindow, &mRenderer)) {
        std::clog << "Failed to init Window and/or Renderer " << SDL_GetError()
            << std::endl;
    }

    mRenderTextureBuffer =
        SDL_CreateTexture(mRenderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_STREAMING,
            mRendererWidth, mRendererHeight);
}

Renderer::~Renderer() {
    SDL_DestroyTexture(mRenderTextureBuffer);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
}

int32_t Renderer::gameLoop(Camera& camera_, Map& map) {
    double time = 0, oldTime = 0;
    double sRotSpeed;
    double cRotSpeed;

    double neg_sRotSpeed;
    double neg_cRotSpeed;
    const bool* keyState = SDL_GetKeyboardState(nullptr);

    std::unique_ptr<uint32_t> renderBuffer(
        new uint32_t[mRendererHeight * mRendererWidth]);
    memset(renderBuffer.get(), 0,
        mRendererWidth * mRendererHeight * sizeof(uint32_t));

    uint32_t texWidth = DEFAULT_TEXTWIDTH, texHeight = DEFAULT_TEXTHEIGHT;
    TextureList textureList;
    textureList.resize(8);

    std::clog << "Working dir: " << std::filesystem::current_path() << '\n';
    for (uint32_t textureIndex = 0;
        const auto& entry :
    std::filesystem::directory_iterator(TEXTURE_ASSET_PATH)) {
        std::filesystem::path outfilename = entry.path();
        textureList[textureIndex].addTexturePNG(
            outfilename.string(), DEFAULT_TEXTWIDTH, DEFAULT_TEXTHEIGHT);
        auto name = stripFromLeft(outfilename.string(), TEXTURE_ASSET_PATH);
        std::clog << "Compiled texture: " << std::setw(50) << std::left << name
            << " || Index: " << std::right << textureIndex++ << '\n';
    }

    std::clog << "Successfully compiled all the textures.\n";

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        SDL_PollEvent(&event);
        const bool* keyState = SDL_GetKeyboardState(nullptr);

        // FLOOR CASTING
        for (int y = 0; y < mRendererHeight; y++) {
            // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
            float rayDirX0 = camera_.dirX - camera_.planeX;
            float rayDirY0 = camera_.dirY - camera_.planeY;
            float rayDirX1 = camera_.dirX + camera_.planeX;
            float rayDirY1 = camera_.dirY + camera_.planeY;

            // Current y position compared to the center of the screen (the horizon)
            int p = y - mRendererHeight / 2;

            // Vertical position of the camera.
            float posZ = 0.5 * mRendererHeight;

            float rowDistance = posZ / p;

            float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / mRendererWidth;
            float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / mRendererWidth;

            float floorX = camera_.posX + rowDistance * rayDirX0;
            float floorY = camera_.posY + rowDistance * rayDirY0;

            for (int x = 0; x < mRendererWidth; ++x) {
                int cellX = (int)(floorX);
                int cellY = (int)(floorY);

                int texX = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
                int texY = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

                floorX += floorStepX;
                floorY += floorStepY;

                unsigned int floorTexture = 7;
                unsigned int ceilingTexture = 5;
                Uint32 color;

                // floor
                color = textureList[floorTexture][texHeight * texY + texX];
                renderBuffer.get()[y * mRendererWidth + x] = color;

                // ceiling
                color = textureList[ceilingTexture][texHeight * texY + texX];
                renderBuffer.get()[(mRendererHeight * mRendererWidth -
                    (y + 1) * mRendererWidth) +
                    x] = color;
            }
        }

        // Wall casting
        for (unsigned int x = 0; x < mRendererWidth; ++x) {
            double mCameraX = 2 * x / double(mRendererWidth) - 1;
            double rayDirX = camera_.dirX + camera_.planeX * mCameraX;
            double rayDirY = camera_.dirY + camera_.planeY * mCameraX;

            // coordinates of the camera_ on the 2D array
            int mapX = int(camera_.posX);
            int mapY = int(camera_.posY);

            // length of ray from current camera_ position to next x or y-side
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
                sideDistX = (camera_.posX - mapX) * deltaDistX;
            }
            else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - camera_.posX) * deltaDistX;
            }
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (camera_.posY - mapY) * deltaDistY;
            }
            else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - camera_.posY) * deltaDistY;
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

                if (map[mapX * map.mMapHeight + mapY] > 0)
                    hit = 1;
            }

            // Calculate distance projected on camera_ direction
            if (side == 0)
                perpWallDist = (sideDistX - deltaDistX);
            else
                perpWallDist = (sideDistY - deltaDistY);

            unsigned int h = mRendererHeight;
            int lineHeight = (int)(h / perpWallDist);

            int drawStart = -lineHeight / 2 + h / 2;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + h / 2;
            if (drawEnd >= h)
                drawEnd = h - 1;

            int texNum = map[mapX * map.mMapWidth + mapY] - 1;

            double wallX;
            if (side == 0)
                wallX = camera_.posY + perpWallDist * rayDirY;
            else
                wallX = camera_.posX + perpWallDist * rayDirX;
            wallX -= floor((wallX));

            int texX = int(wallX * double(texWidth));
            if (side == 0 && rayDirX > 0)
                texX = texWidth - texX - 1;
            if (side == 1 && rayDirY < 0)
                texX = texWidth - texX - 1;

            // How much to increase the texture coordinate per screen pixel
            double step = 1.0 * texHeight / lineHeight;
            double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
            for (int y = drawStart; y < drawEnd; y++) {
                int texY = (int)texPos & (texHeight - 1);
                texPos += step;
                Uint32 color;
                if (texNum > textureList.size() - 1) {
                    color = textureList[textureList.size() - 1][texHeight * texY + texX];
                }
                else {
                    color = textureList[texNum][texHeight * texY + texX];
                }
                if (side == 0) {
                    RGBAPixel colorRGB = makeRGBA8888(color);
                    renderBuffer.get()[y * mRendererWidth + x] =
                        makeRGBA8888(colorRGB.r, colorRGB.g, colorRGB.b, 170);
                }
                else {
                    renderBuffer.get()[y * mRendererWidth + x] = color;
                }
            }
        }

        SDL_RenderClear(mRenderer);
        if (SDL_UpdateTexture(mRenderTextureBuffer, nullptr, renderBuffer.get(),
            mRendererWidth * sizeof(uint32_t)) < 0) {
            std::clog << "UpdateTexture failed: " << SDL_GetError() << std::endl;
        }

        if (SDL_RenderTexture(mRenderer, mRenderTextureBuffer, nullptr, nullptr) <
            0) {
            std::clog << "RenderTexture failed: " << SDL_GetError() << std::endl;
        }

        oldTime = time;
        time = SDL_GetTicks();
        double frameTime = (time - oldTime) / 1000.0;
        std::clog << "\r" << (frameTime) << " ms" << std::flush;

        if (frameTime < 0.001f)
            frameTime = 0.001f;
        if (frameTime > 0.05f)
            frameTime = 0.05f;

        sRotSpeed = sin(camera_.rotationSpeedXY * frameTime * 5);
        cRotSpeed = cos(camera_.rotationSpeedXY * frameTime * 5);

        neg_sRotSpeed = sin(-camera_.rotationSpeedXY * frameTime * 5);
        neg_cRotSpeed = cos(-camera_.rotationSpeedXY * frameTime * 5);

        // User requests quit
        if (keyState[SDL_SCANCODE_ESCAPE] || keyState[SDL_SCANCODE_BACKSPACE] ||
            event.type == SDL_EVENT_QUIT)
            quit = true;

        if (keyState[SDL_SCANCODE_UP]) {
            if (!map[int(camera_.posX + camera_.dirX * camera_.speedXY) *
                map.mMapHeight +
                int(camera_.posY)])
                camera_.posX += camera_.dirX * camera_.speedXY;
            if (!map[int(camera_.posX) * map.mMapHeight +
                int(camera_.posY + camera_.dirY * camera_.speedXY)])
                camera_.posY += camera_.dirY * camera_.speedXY;
        }

        if (keyState[SDL_SCANCODE_DOWN]) {
            if (!map[int(camera_.posX - camera_.dirX * camera_.speedXY) *
                map.mMapHeight +
                int(camera_.posY)])
                camera_.posX -= camera_.dirX * camera_.speedXY;
            if (!map[int(camera_.posX) * map.mMapHeight +
                int(camera_.posY - camera_.dirY * camera_.speedXY)])
                camera_.posY -= camera_.dirY * camera_.speedXY;
        }

        if (keyState[SDL_SCANCODE_RIGHT]) {
            double oldDirX = camera_.dirX;
            camera_.dirX =
                camera_.dirX * neg_cRotSpeed - camera_.dirY * neg_sRotSpeed;
            camera_.dirY = oldDirX * neg_sRotSpeed + camera_.dirY * neg_cRotSpeed;
            double oldPlaneX = camera_.planeX;
            camera_.planeX =
                camera_.planeX * neg_cRotSpeed - camera_.planeY * neg_sRotSpeed;
            camera_.planeY =
                oldPlaneX * neg_sRotSpeed + camera_.planeY * neg_cRotSpeed;
        }
        if (keyState[SDL_SCANCODE_LEFT]) {
            double oldDirX = camera_.dirX;
            camera_.dirX = camera_.dirX * cRotSpeed - camera_.dirY * sRotSpeed;
            camera_.dirY = oldDirX * sRotSpeed + camera_.dirY * cRotSpeed;
            double oldPlaneX = camera_.planeX;
            camera_.planeX = camera_.planeX * cRotSpeed - camera_.planeY * sRotSpeed;
            camera_.planeY = oldPlaneX * sRotSpeed + camera_.planeY * cRotSpeed;
        }
        SDL_RenderPresent(mRenderer);
    }
    return 0;
}
