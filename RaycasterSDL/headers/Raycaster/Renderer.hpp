#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <Raycaster.hpp>
#include <stdint.h>

class Map;
class Camera;

class Renderer {
private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    SDL_Texture* mRenderTextureBuffer;
    uint32_t mRendererWidth, mRendererHeight;

public:
    Renderer();

    Renderer(uint32_t width, uint32_t height)
        : mWindow(nullptr), mRenderer(nullptr), mRendererWidth(width),
        mRendererHeight(height) {};

    ~Renderer();

    int32_t init(std::string_view appName);

    int32_t render(Camera& camera_, Map& map_);
};

#endif