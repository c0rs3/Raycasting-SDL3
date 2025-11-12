#pragma once
#ifndef RAYCASTER_TEXTURE
#define RAYCASTER_TEXTURE

#include <Raycaster.hpp>
#define TEXTURE_ASSET_PATH "RaycasterSDL/assets/textures/"

uint32_t makeRGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
struct RGBPixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

std::string stripString(const std::string& iString, const std::string& ToStrip);

class Texture {
public:
    Texture();
    ~Texture();
    void addTexturePNG(const std::string& filePath, unsigned int textHeight, 
        unsigned int textWidth);
    uint32_t operator[](size_t index);
    void clearTextures();
public:
    std::vector<std::string> textureNameList;
    unsigned int mHeight;
    unsigned int mWidth;
    std::vector<uint32_t> mData;
private:
};


std::vector<RGBPixel> loadPNG(const std::string& filename);

std::ostream& operator<<(std::ostream& stream, RGBPixel pixel);

#endif
