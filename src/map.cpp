#include "map.h"
#include "raycaster.h"

extern uint32_t makeRGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

Map::~Map() {
    delete[] mData;
}

void Map::addMap(const unsigned int& height, const unsigned int& width) {
    mapWidth = width; mapHeight = height;
    if (mData == nullptr)
        mData = new int[mapWidth * mapHeight];

    printMap();
    unsigned int inputVal;
    for (unsigned int w = 0; w < mapWidth; w++) {
        for (unsigned int h = 0; h < mapHeight; h++) {
            std::cin >> inputVal;
            mData[w * mapWidth + h] = inputVal;
            system("clear");
            printMap();
        }
    }
}

void Map::printMap() {
    for (unsigned int w = 0; w < mapWidth; w++) {
        std::cout << "{";
        for (unsigned int h = 0; h < mapHeight; h++) {
            std::cout << mData[w * mapWidth + h] << ((h == mapHeight - 1) ? "" : ", ");
        }
        std::cout << ((w == mapHeight - 1) ? "}" : "},") << std::endl;
    }
}

int Map::UI(unsigned int screenWidth, unsigned int screenHeight) {
    this->printMap();
    SDL_Window* WindowContext;
    SDL_Renderer* RenderContext;

    uint32_t* renderBuffer = new uint32_t[screenHeight * screenWidth];

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

        unsigned int heightStep = screenHeight / mapHeight;
        unsigned int widthStep = screenWidth / mapWidth;
        // unsigned int stepGlobal = (heightStep > widthStep) ? widthStep : heightStep;

        for (size_t w = 0; w < screenWidth; w++) {
            for (size_t h = 0; h < screenHeight; h++) {
                unsigned int mapX = static_cast<unsigned int>(w / widthStep);
                unsigned int mapY = static_cast<unsigned int>(h / heightStep);
                if (mData[(mapX * w / widthStep) + (mapY * h / heightStep)]) {
                    /*
                    std::clog << "Map" << mData[(mapX * w / widthStep) + mapY] 
                    << " w" << w
                    << " h" << h << std::endl;
                    */
                    renderBuffer[screenWidth * h + w] = makeRGBA8888(0, 0, 255, 255);
                }
            }
        }

        if (SDL_UpdateTexture(renderTexture, nullptr, renderBuffer,
            screenWidth * sizeof(uint32_t)) < 0) {
            std::clog << "UpdateTexture failed: " << SDL_GetError() << std::endl;
        }

        if (SDL_RenderTexture(RenderContext, renderTexture, nullptr, nullptr) < 0) {
            std::clog << "RenderTexture failed: " << SDL_GetError() << std::endl;
        }

        if (keyState[SDL_SCANCODE_ESCAPE] || keyState[SDL_SCANCODE_BACKSPACE]) {
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
