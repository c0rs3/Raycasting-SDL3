#pragma once
#include <Raycaster/Entity.hpp>
#include <Raycaster/map.hpp>

#ifndef	RAYCASTER_RENDERER
#define RAYCASTER_RENDERER

class Camera {
public:
    Camera(double posX = 0, double posY = 0, double dirX = 0,
        double dirY = 0, double planeX = 0, double planeY = 0.66f, double moveSpeed = 0.1f, double rotSpeed = 0.1f)
        : posX(posX), posY(posY), dirX(dirX),
        dirY(dirY), planeX(planeX), planeY(planeY),
        moveSpeed(moveSpeed), rotSpeed(rotSpeed) {};
    ~Camera() {};

    double posX, posY;
    double dirX, dirY;
    double planeX, planeY; // Vector of the camera plane
    double moveSpeed, rotSpeed;
};

class Renderer {
public:
    Renderer(const Camera& camera, unsigned int screenWidth, unsigned int screenHeight)
        :mCamera(camera), mWindowContext(nullptr), mRenderContext(nullptr), screenWidth(screenWidth), screenHeight(screenHeight) {}

    ~Renderer() = default;

    int render(Map& map);

private:
    Camera mCamera;
    SDL_Window* mWindowContext;
    SDL_Renderer* mRenderContext;
    unsigned int screenWidth, screenHeight;
};
#endif