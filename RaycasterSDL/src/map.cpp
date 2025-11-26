#include <Raycaster.hpp>
#include <Raycaster/Map.hpp>


#include <fstream>
#include <iostream>


static constexpr uint32_t textureLookup[] = {
    BBLACK, BBLUE,   BGREEN, BRED,    BORANGE, BPURPLE, BTEAL,
    BOLIVE, BMAROON, BNAVY,  BSILVER, BGRAY,   BTRANS };

Map::Map() = default;

Map::~Map() = default;

uint32_t Map::getSection(size_t section) {
    _ASSERT(section >= 0 && section < mMapHeight * mMapWidth);
    return mData.get()[section];
};

uint32_t Map::getSection(size_t x, size_t y) {
    _ASSERT(x < mMapWidth && x >= 0 && y < mMapHeight && y >= 0);
    return mData.get()[y * mMapWidth + x];
};

uint32_t& Map::operator[](size_t section) {
    _ASSERT(section >= 0 && section < mMapHeight * mMapWidth);
    return mData.get()[section];
};

uint32_t Map::getWidth() {
    return mMapWidth;
}

uint32_t Map::getHeight() {
    return mMapHeight;
}

void Map::setSection(uint32_t x, uint32_t y, uint32_t val) {
    _ASSERT(x < mMapWidth && x >= 0 && y < mMapHeight && y >= 0);
    mData.get()[y * mMapWidth + x] = val;
};

void Map::storeToFile(std::string_view path) {
    // todo
};

void Map::readFromFile(std::string_view path) {
    // todo
};

void Map::EditorCLI(uint32_t width, uint32_t height) {
    mMapHeight = width;
    mMapHeight = height;
    printMap();
    uint32_t inputVal;
    for (uint32_t h = 0; h < mMapHeight; h++) {
        for (uint32_t w = 0; w < mMapWidth; w++) {
            std::cin >> inputVal;
            mData.get()[h * mMapWidth + w] = inputVal;
            system("clear");
            printMap();
        }
    }
};

int Map::EditorWindow(uint32_t screenWidth, uint32_t screenHeight) {
    SDL_Window* WindowContext;
    SDL_Renderer* RenderContext;

    uint32_t mapSection;
    // double oldTime, time;

    std::unique_ptr<uint32_t> renderBuffer(
        new uint32_t[screenHeight * screenWidth]);
    std::memset(renderBuffer.get(), 0, screenHeight * screenWidth);

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

        uint32_t heightStep = screenHeight / mMapHeight;
        uint32_t widthStep = screenWidth / mMapWidth;
        // heightStep = widthStep > heightStep ? heightStep: widthStep;
        // widthStep  = heightStep > widthStep ? heightStep: widthStep;
        for (size_t h = 0; h < screenHeight; h++) {
            for (size_t w = 0; w < screenWidth; w++) {
                mapSection = ((h / heightStep) * mMapWidth + (w / widthStep));
                if (mapSection >= mMapWidth * mMapHeight) {
                    mapSection = mMapWidth * mMapHeight - 1;
                }
                auto drawColor = [h, w, &renderBuffer,
                    screenWidth](uint32_t xSize, uint32_t RGBAcolor) {
                    for (size_t i = 0; i < xSize; i++) {
                        renderBuffer.get()[screenWidth * h + w + i] = RGBAcolor;
                    }
                    };

                if ((mMapWidth * int(ySelectedOld) + int(xSelectedOld))) {
                    drawColor(5, BBLACK);
                }
                if (mapSection == (mMapWidth * int(ySelected) + int(xSelected))) {
                    drawColor(5, BWHITE);
                }
                else {
                    drawColor(5, textureLookup[mData.get()[mapSection]]);
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
        if (keyState[SDL_SCANCODE_BACKSPACE]) {
            mData.get()[(mMapWidth * int(ySelected) + int(xSelected))] = 0;
        }
        if (keyState[SDL_SCANCODE_1]) {
            mData.get()[(mMapWidth * int(ySelected) + int(xSelected))] = 1;
        }
        if (keyState[SDL_SCANCODE_2]) {
            mData.get()[(mMapWidth * int(ySelected) + int(xSelected))] = 2;
        }
        if (keyState[SDL_SCANCODE_3]) {
            mData.get()[(mMapWidth * int(ySelected) + int(xSelected))] = 3;
        }
        if (keyState[SDL_SCANCODE_4]) {
            mData.get()[(mMapWidth * int(ySelected) + int(xSelected))] = 4;
        }
        if (keyState[SDL_SCANCODE_5]) {
            mData.get()[(mMapWidth * int(ySelected) + int(xSelected))] = 5;
        }
        if (keyState[SDL_SCANCODE_6]) {
            mData.get()[(mMapWidth * int(ySelected) + int(xSelected))] = 6;
        }
        if (keyState[SDL_SCANCODE_7]) {
            mData.get()[(mMapWidth * int(ySelected) + int(xSelected))] = 7;
        }

        // std::clog << "\r" << (xSelected) << " " << (ySelected) << std::endl;

        if (xSelected >= mMapWidth)
            xSelected = mMapWidth - 1;
        if (ySelected >= mMapHeight)
            ySelected = mMapHeight - 1;
        if (ySelected < 0)
            ySelected = ySelectedOld;
        if (xSelected < 0)
            xSelected = xSelectedOld;

        if (keyState[SDL_SCANCODE_ESCAPE] || event.type == SDL_EVENT_QUIT)
            goto sdlexit;
        SDL_RenderPresent(RenderContext);

        // oldTime = time;
        // time = SDL_GetTicks();
        // double frameTime = (time - oldTime) / 1000.0;
    }

    goto sdlexit;
sdlexit:
    SDL_DestroyRenderer(RenderContext);
    SDL_DestroyWindow(WindowContext);
    return 0;
};

void Map::printMap() {
    for (uint32_t w = 0; w < mMapWidth; w++) {
        std::cout << "{";
        for (uint32_t h = 0; h < mMapHeight; h++) {
            std::cout << mData.get()[w * mMapHeight + h]
                << ((h == mMapHeight - 1) ? "" : ", ");
        }
        std::cout << ((w == mMapHeight - 1) ? "}" : "},") << std::endl;
    }
    std::cout << std::endl;
};