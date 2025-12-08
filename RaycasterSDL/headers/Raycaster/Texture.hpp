#pragma once

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

struct RGBAPixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;
};

std::ostream& operator<<(std::ostream& stream, RGBAPixel pixel);

uint32_t makeRGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

RGBAPixel makeRGBA8888(uint32_t rgba8888);

std::string stripFromLeft(const std::string& str_, const std::string& toStrip);

std::vector<RGBAPixel> loadPNG(std::string_view path);

class Texture {
private:
    std::vector<uint32_t> mData;
    uint32_t mTexHeight;
    uint32_t mTexWidth;

public:
    void addTexturePNG(const std::string& filePath, uint32_t textHeight,
        uint32_t textWidth);
    uint32_t operator[](size_t index);
};

using TextureList = std::vector<Texture>;
