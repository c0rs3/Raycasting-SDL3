#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <utility>

class Camera {
private:
public:
    float posX, posY;
    float dirX, dirY;
    float planeX, planeY;
    float speedXY, rotationSpeedXY;

public:
    Camera();
    Camera(double posX = 0, double posY = 0, double dirX = 0, double dirY = 0,
        double planeX = 0, double planeY = 0.66f, double moveSpeed = 0.1f,
        double rotSpeed = 0.1f)
        : posX(posX), posY(posY), dirX(dirX), dirY(dirY), planeX(planeX),
        planeY(planeY), speedXY(moveSpeed), rotationSpeedXY(rotSpeed) {};

    ~Camera();

    std::pair<float, float> getPos();

    void setPos(float posX_, float posY_);

    std::pair<float, float> getDir();

    void setDir(float dirX_, float dirY_);

    std::pair<float, float> getPlane();

    void setPlane(float planeX_, float planeY_);

    float getSpeed();

    void setSpeed(float speed_);

    float getRotationSpeed();

    void setRotationSpeed(float rotation_);
};

#endif