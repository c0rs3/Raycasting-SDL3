#include "renderer.h"

Renderer::~Renderer() {
    delete[] buffer;
}

int Renderer::render(Map& map) {
    buffer = new uint32_t[screenHeight * screenWidth];
    memset(buffer, 0, screenWidth * screenHeight * sizeof(uint32_t));
    std::vector<Texture> textureList = std::vector<Texture>();
    textureList.resize(8);
    /*
    for (unsigned int i = 0; i < 8; i++)
    textureList[i].addTexturePNG("assets/brown.png", texHeight, texWidth);

    */
    unsigned int fileIterationCount = 0;
    for (const auto& entry : std::filesystem::directory_iterator("assets")) {
        std::filesystem::path outfilename = entry.path();
        
        std::clog << "Compiled texture: " << outfilename.string()<< std::endl;
        textureList[fileIterationCount].addTexturePNG(outfilename.string(), texHeight, texWidth);

        fileIterationCount++;
    }


    double time = 0, oldTime = 0;
    double sRotSpeed;
    double cRotSpeed;

    double neg_sRotSpeed;
    double neg_cRotSpeed;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::clog << "Failed to init SDL video: "
            << SDL_GetError() << std::endl;
        return -1;
    }

    if (!SDL_CreateWindowAndRenderer("Ray-casting", screenWidth,
        screenHeight, 0, &mWindow_context, &mRender_context)) {
        std::clog << "Failed to init Window and/or Renderer"
            << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Texture* render_texture = SDL_CreateTexture(
        mRender_context, PIXEL_FORMAT, SDL_TEXTUREACCESS_STREAMING,
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

            int texNum = map.mData[mapX][mapY] - 1;

            double wallX;
            if (side == 0) wallX = mCamera.posY + perpWallDist * rayDirY;
            else           wallX = mCamera.posX + perpWallDist * rayDirX;
            wallX -= floor((wallX));

            int texX = int(wallX * double(texWidth));
            if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
            if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

            // How much to increase the texture coordinate per screen pixel
            double step = 1.0 * texHeight / lineHeight;
            double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
            for (int y = drawStart; y < drawEnd; y++) {
                int texY = (int)texPos & (texHeight - 1);
                texPos += step;
                Uint32 color;
                if (texNum > textureList.size() - 1) {
                    color = textureList[textureList.size() - 1].mData[texHeight * texY + texX];
                }
                else {
                    color = textureList[texNum].mData[texHeight * texY + texX];
                }
                buffer[y * screenWidth + x] = color;
            }
        }
        SDL_RenderClear(mRender_context);
        if (SDL_UpdateTexture(render_texture, nullptr, buffer,
            screenWidth * sizeof(uint32_t)) < 0) {
            std::clog << "UpdateTexture failed: " << SDL_GetError() << std::endl;
        }

        if (SDL_RenderTexture(mRender_context, render_texture, nullptr, nullptr) < 0) {
            std::clog << "RenderTexture failed: " << SDL_GetError() << std::endl;
        }

        oldTime = time;
        time = SDL_GetTicks();
        double frameTime = (time - oldTime) / 1000.0;
        if (int(SDL_GetTicks() / 1000) == 1)
            std::clog << "\r" << (1.0 / frameTime) << std::flush;

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
