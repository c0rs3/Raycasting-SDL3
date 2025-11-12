#include <Raycaster.hpp>
#include <Raycaster/Map.hpp>

extern uint32_t makeRGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

uint32_t textureLookup[] = { BBLACK, BBLUE, BGREEN, BRED, BORANGE, BPURPLE, BTEAL, BOLIVE, BMAROON, BNAVY, BSILVER, BGRAY, BTRANS };

void Map::printMap() {
    for (uint32_t w = 0; w < mapWidth; w++) {
        std::cout << "{";
        for (uint32_t h = 0; h < mapHeight; h++) {
            std::cout << mData.get()[w * mapWidth + h] << ((h == mapHeight - 1) ? "" : ", ");
        }
        std::cout << ((w == mapHeight - 1) ? "}" : "},") << std::endl;
    }
    std::cout << std::endl;
}

void Map::EditorCLI(uint32_t height, uint32_t width) {
    mapWidth = width; mapHeight = height;
    printMap();
    uint32_t inputVal;
    for (uint32_t w = 0; w < mapWidth; w++) {
        for (uint32_t h = 0; h < mapHeight; h++) {
            std::cin >> inputVal;
            mData.get()[w * mapWidth + h] = inputVal;
            system("clear");
            printMap();
        }
    }
}

int Map::EditorWindow(uint32_t screenWidth, uint32_t screenHeight) {
    SDL_Window* WindowContext;
    SDL_Renderer* RenderContext;

    uint32_t mapSection;
    // double oldTime, time;

    std::unique_ptr<uint32_t> renderBuffer(new uint32_t[screenHeight * screenWidth]);
    std::memset(renderBuffer.get(), 0, screenHeight * screenWidth);

    float xSelected = 0, ySelected = 0;
    float xSelectedOld = 0, ySelectedOld = 0;

    const bool* keyState = SDL_GetKeyboardState(nullptr);
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::clog << "Failed to init SDL video: "
            << SDL_GetError() << std::endl;

        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("Map Editor", screenWidth,
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
        SDL_RenderClear(RenderContext);
        SDL_Event event;
        SDL_PollEvent(&event);

        uint32_t heightStep = screenHeight / mapHeight;
        uint32_t widthStep = screenWidth / mapWidth;
        // heightStep = widthStep > heightStep ? heightStep: widthStep;
        // widthStep  = heightStep > widthStep ? heightStep: widthStep;
        for (size_t h = 0; h < screenHeight; h++) {
            for (size_t w = 0; w < screenWidth; w++) {
                mapSection = ((h / heightStep) * mapWidth + (w / widthStep));
                if (mapSection >= mapWidth * mapHeight) {
                    mapSection = mapWidth * mapHeight - 1;
                }
                auto drawColor = [h, w, &renderBuffer, screenWidth](uint32_t xSize, uint32_t RGBAcolor) {
                    for (size_t i = 0; i < xSize; i++) {
                        renderBuffer.get()[screenWidth * h + w + i] = RGBAcolor;
                    }
                };

                if ((mapWidth * int(ySelectedOld) + int(xSelectedOld))) { drawColor(5, BBLACK); }
                if (mapSection == (mapWidth * int(ySelected) + int(xSelected))) { drawColor(5, BWHITE); }
                else { drawColor(5, textureLookup[mData.get()[mapSection]]); }
            }
        }


        // Render buffer updating and rendering
        if (SDL_UpdateTexture(renderTexture, nullptr, renderBuffer.get(),
            screenWidth * sizeof(uint32_t)) < 0) std::clog << "UpdateTexture failed: " << SDL_GetError() << std::endl;

        if (SDL_RenderTexture(RenderContext, renderTexture, nullptr, nullptr) < 0) std::clog << "RenderTexture failed: " << SDL_GetError() << std::endl;

        // Cursor movement
        if (keyState[SDL_SCANCODE_W]) { ySelectedOld = ySelected; ySelected -= 0.5; }
        if (keyState[SDL_SCANCODE_A]) { xSelectedOld = xSelected; xSelected -= 0.5; }
        if (keyState[SDL_SCANCODE_S]) { ySelectedOld = ySelected; ySelected += 0.5; }
        if (keyState[SDL_SCANCODE_D]) { xSelectedOld = xSelected; xSelected += 0.5; }

        // Map selection texture setting
        if (keyState[SDL_SCANCODE_BACKSPACE]) { mData.get()[(mapWidth * int(ySelected) + int(xSelected))] = 0; }
        if (keyState[SDL_SCANCODE_1]) { mData.get()[(mapWidth * int(ySelected) + int(xSelected))] = 1; }
        if (keyState[SDL_SCANCODE_2]) { mData.get()[(mapWidth * int(ySelected) + int(xSelected))] = 2; }
        if (keyState[SDL_SCANCODE_3]) { mData.get()[(mapWidth * int(ySelected) + int(xSelected))] = 3; }
        if (keyState[SDL_SCANCODE_4]) { mData.get()[(mapWidth * int(ySelected) + int(xSelected))] = 4; }
        if (keyState[SDL_SCANCODE_5]) { mData.get()[(mapWidth * int(ySelected) + int(xSelected))] = 5; }
        if (keyState[SDL_SCANCODE_6]) { mData.get()[(mapWidth * int(ySelected) + int(xSelected))] = 6; }
        if (keyState[SDL_SCANCODE_7]) { mData.get()[(mapWidth * int(ySelected) + int(xSelected))] = 7; }

        // std::clog << "\r" << (xSelected) << " " << (ySelected) << std::endl;

        if (xSelected >= mapWidth) xSelected = mapWidth - 1;
        if (ySelected >= mapHeight) ySelected = mapHeight - 1;
        if (ySelected < 0) ySelected = ySelectedOld;
        if (xSelected < 0) xSelected = xSelectedOld;


        if (keyState[SDL_SCANCODE_ESCAPE] || event.type == SDL_EVENT_QUIT) goto sdlexit;
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
}
