#include "map.h"
#include "raycaster.h"

Map::~Map() {
    for (size_t i = 0; i < mapWidth; i++)
        delete[] mData[i];
    delete[] mData;
}

void Map::addMap(const unsigned int& height, const unsigned int& width) {
    mapWidth = width;
    mapHeight = height;
    if (mData == nullptr) {
        mData = new int* [mapWidth];
        for (unsigned int i = 0; i < mapWidth; ++i)
            mData[i] = new int[mapHeight];
    }

    printMap();
    unsigned int inputVal;
    for (unsigned int i = 0; i < mapWidth; i++) {
        for (unsigned int k = 0; k < mapHeight; k++) {
            std::cin >> inputVal;
            mData[i][k] = inputVal;
            system("clear");
            printMap();
        }
    }
}

void Map::printMap() {
    for (unsigned int i = 0; i < mapWidth; i++) {
        std::cout << "{";
        for (unsigned int k = 0; k < mapHeight; k++) {
            std::cout << mData[i][k] << ((k == mapHeight - 1) ? "" : ", ");
        }
        std::cout << ((i == mapHeight - 1) ? "}" : "},") << std::endl;
    }
}

int Map::UI(unsigned int screenWidth, unsigned int screenHeight) {
    this->printMap();
    SDL_Window* WindowContext;
    SDL_Renderer* RenderContext;

    uint32_t* renderBuffer = new uint32_t[screenHeight * screenWidth];
    memset(renderBuffer, 0, screenHeight * screenWidth);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::clog << "Failed to init SDL video: "
            << SDL_GetError() << std::endl;
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("Ray-casting", screenWidth,
        screenHeight, 0, &WindowContext, &RenderContext)) {
        std::clog << "Failed to init Window and/or Renderer"
            << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* renderTexture = SDL_CreateTexture(
        RenderContext, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        screenWidth, screenHeight);

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        SDL_PollEvent(&event);
        const bool* keyState = SDL_GetKeyboardState(nullptr);
        SDL_RenderClear(RenderContext);
        
        for (size_t i = 0; i < mapHeight; ++i) {
            for (size_t k = 0; k < mapWidth; ++k) {
                // 
            }
        }

        if (SDL_UpdateTexture(renderTexture, nullptr, renderBuffer,
            screenWidth * sizeof(uint32_t)) < 0) {
            std::clog << "UpdateTexture failed: " << SDL_GetError() << std::endl;
        }

        if (SDL_RenderTexture(RenderContext, renderTexture, nullptr, nullptr) < 0) {
            std::clog << "RenderTexture failed: " << SDL_GetError() << std::endl;
        }

        if (event.type == SDL_EVENT_QUIT || event.key.scancode == SDL_SCANCODE_ESCAPE) {
            quit = true;
            SDL_DestroyWindow(WindowContext);
            SDL_DestroyRenderer(RenderContext);
            SDL_DestroyTexture(renderTexture);
        }

        SDL_RenderPresent(RenderContext);
    }

    SDL_DestroyRenderer(RenderContext);
    SDL_DestroyWindow(WindowContext);
    SDL_DestroyTexture(renderTexture);
    return 0;
}
