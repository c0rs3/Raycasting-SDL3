#include <Raycaster/Map.hpp>

#include <assert.h>
#include <fstream>
#include <iostream>

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
#include <SDL3_Image/SDL_image.h>

#define _ASSERT assert

static constexpr uint32_t textureLookup[] = {
    BBLACK, BBLUE,   BGREEN, BRED,    BORANGE, BPURPLE, BTEAL,
    BOLIVE, BMAROON, BNAVY,  BSILVER, BGRAY,   BTRANS };

int32_t& Map::getSection(size_t section) {
    _ASSERT(section >= 0 && section < mMapHeight * mMapWidth);
    return mData[section];
};

int32_t& Map::getSection(size_t x, size_t y) {
    _ASSERT(x < mMapWidth && x >= 0 && y < mMapHeight && y >= 0);
    return mData[y * mMapWidth + x];
};

int32_t& Map::operator[](size_t section) {
    _ASSERT(section >= 0 && section < mMapHeight * mMapWidth);
    return mData[section];
};

uint32_t Map::getWidth() const { return mMapWidth; }

uint32_t Map::getHeight() const { return mMapHeight; }

void Map::setSection(uint32_t x, uint32_t y, int32_t val) {
    _ASSERT(x < mMapWidth && x >= 0 && y < mMapHeight && y >= 0);
    mData[y * mMapWidth + x] = val;
};

void EditorConsole(Map& map) {
    logMapToConsole(map);
    uint32_t inputVal;
    for (uint32_t w = 0; w < map.getWidth(); w++) {
        for (uint32_t h = 0; h < map.getHeight(); h++) {
            std::cin >> inputVal;
            map[w * map.mMapWidth + h] = inputVal;
            system("clear");
            logMapToConsole(map);
        }
    }
};

int EditorWindow(Map& map, uint32_t screenWidth, uint32_t screenHeight) {
    SDL_Window* WindowContext;
    SDL_Renderer* RenderContext;

    std::unique_ptr<uint32_t> renderBuffer(
        new uint32_t[screenHeight * screenWidth]);
    memset(renderBuffer.get(), 0, screenHeight * screenWidth);

    uint32_t mapSection;
    float xSelected = 0, ySelected = 0;
    float xSelectedOld = 0, ySelectedOld = 0;

    const bool* keyState = SDL_GetKeyboardState(nullptr);
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::clog << "Failed to init SDL video: " << SDL_GetError() << std::endl;

        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("Map Editor", screenWidth, screenHeight, 0,
        &WindowContext, &RenderContext)) {
        std::clog << "Failed to init Window and/or Renderer" << SDL_GetError()
            << std::endl;
        return 1;
    }

    SDL_Texture* renderTexture =
        SDL_CreateTexture(RenderContext, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

    bool quit = false;
    while (!quit) {
        SDL_RenderClear(RenderContext);
        SDL_Event event;
        SDL_PollEvent(&event);

        uint32_t heightStep = screenHeight / map.mMapHeight;
        uint32_t widthStep = screenWidth / map.mMapWidth;
        for (size_t h = 0; h < screenHeight; h++) {
            for (size_t w = 0; w < screenWidth; w++) {
                mapSection = ((h / heightStep) * map.mMapWidth + (w / widthStep));
                if (mapSection >= map.mMapWidth * map.mMapHeight) {
                    mapSection = map.mMapWidth * map.mMapHeight - 1;
                }
                auto drawColor = [h, w, &renderBuffer,
                    screenWidth](uint32_t xSize, uint32_t RGBAcolor) {
                    for (size_t i = 0; i < xSize; i++) {
                        renderBuffer.get()[screenWidth * h + w + i] = RGBAcolor;
                    }
                    };

                if ((map.mMapWidth * int(ySelectedOld) + int(xSelectedOld))) {
                    drawColor(5, BBLACK);
                }
                if (mapSection == (map.mMapWidth * int(ySelected) + int(xSelected))) {
                    drawColor(5, BWHITE);
                }
                else {
                    drawColor(5, textureLookup[map[mapSection]]);
                }
            }
        }

        // Render buffer updating and rendering
        if (SDL_UpdateTexture(renderTexture, nullptr, renderBuffer.get(),
            screenWidth * sizeof(uint32_t)) < 0)
            std::clog << "UpdateTexture failed: " << SDL_GetError() << std::endl;

        if (SDL_RenderTexture(RenderContext, renderTexture, nullptr, nullptr) < 0)
            std::clog << "RenderTexture failed: " << SDL_GetError() << std::endl;

        // Cursor movement
        if (keyState[SDL_SCANCODE_W]) {
            ySelectedOld = ySelected;
            ySelected -= 0.5;
        }
        if (keyState[SDL_SCANCODE_A]) {
            xSelectedOld = xSelected;
            xSelected -= 0.5;
        }
        if (keyState[SDL_SCANCODE_S]) {
            ySelectedOld = ySelected;
            ySelected += 0.5;
        }
        if (keyState[SDL_SCANCODE_D]) {
            xSelectedOld = xSelected;
            xSelected += 0.5;
        }

        // Map selection texture setting
        if (keyState[SDL_SCANCODE_0]) {
            map[(map.mMapWidth * int(ySelected) + int(xSelected))] = 0;
        }
        if (keyState[SDL_SCANCODE_1]) {
            map[(map.mMapWidth * int(ySelected) + int(xSelected))] = 1;
        }
        if (keyState[SDL_SCANCODE_2]) {
            map[(map.mMapWidth * int(ySelected) + int(xSelected))] = 2;
        }
        if (keyState[SDL_SCANCODE_3]) {
            map[(map.mMapWidth * int(ySelected) + int(xSelected))] = 3;
        }
        if (keyState[SDL_SCANCODE_4]) {
            map[(map.mMapWidth * int(ySelected) + int(xSelected))] = 4;
        }
        if (keyState[SDL_SCANCODE_5]) {
            map[(map.mMapWidth * int(ySelected) + int(xSelected))] = 5;
        }
        if (keyState[SDL_SCANCODE_6]) {
            map[(map.mMapWidth * int(ySelected) + int(xSelected))] = 6;
        }
        if (keyState[SDL_SCANCODE_7]) {
            map[(map.mMapWidth * int(ySelected) + int(xSelected))] = 7;
        }

        if (xSelected >= map.mMapWidth)
            xSelected = map.mMapWidth - 1;
        if (ySelected >= map.mMapHeight)
            ySelected = map.mMapHeight - 1;
        if (ySelected < 0)
            ySelected = ySelectedOld;
        if (xSelected < 0)
            xSelected = xSelectedOld;

        if (keyState[SDL_SCANCODE_ESCAPE] || event.type == SDL_EVENT_QUIT)
            goto sdlexit;
        SDL_RenderPresent(RenderContext);
    }

    goto sdlexit;
sdlexit:
    SDL_DestroyRenderer(RenderContext);
    SDL_DestroyWindow(WindowContext);
    return 0;
};

void logMapToConsole(Map& map) {
    for (uint32_t w = 0; w < map.mMapWidth; w++) {
        std::cout << "{";
        for (uint32_t h = 0; h < map.mMapHeight; h++) {
            std::cout << map[w * map.mMapWidth + h]
                << ((h == map.mMapHeight - 1) ? "" : ", ");
        }
        std::cout << ((w == map.mMapWidth - 1) ? "}" : "},") << std::endl;
    }
    std::cout << std::endl;
};
