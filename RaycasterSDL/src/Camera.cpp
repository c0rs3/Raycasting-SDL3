#include <Raycaster/Camera.hpp>

Camera::Camera() = default;

Camera::~Camera() = default;

std::pair<float, float> Camera::getPos() { return { posX, posY }; };

void Camera::setPos(float posX_, float posY_) {
    posX = posX_;
    posY = posY_;
};

std::pair<float, float> Camera::getDir() { return { dirX, dirY }; };

void Camera::setDir(float dirX_, float dirY_) {
    dirX = dirX_;
    dirY = dirY_;
};

std::pair<float, float> Camera::getPlane() { return { planeX, planeY }; };

void Camera::setPlane(float planeX_, float planeY_) {
    planeX = planeX_;
    planeY = planeY_;
};

float Camera::getSpeed() { return speedXY; };

void Camera::setSpeed(float speed_) { speedXY = speed_; };

float Camera::getRotationSpeed() { return rotationSpeedXY; };

void Camera::setRotationSpeed(float rotation_) { rotationSpeedXY = rotation_; };