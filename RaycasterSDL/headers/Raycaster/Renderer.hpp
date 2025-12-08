#pragma once

#include <string>
#include <stdint.h>

class Map;
class Camera;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Renderer {
private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    SDL_Texture* mRenderTextureBuffer;
    uint32_t mRendererWidth, mRendererHeight;

public:
    Renderer(uint32_t width, uint32_t height);
    
    ~Renderer();

    int32_t gameLoop(Camera& camera_, Map& map_);
};
