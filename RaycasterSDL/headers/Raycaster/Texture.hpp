#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

struct RGBAPixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};


class Texture {
private:
    std::unique_ptr<uint32_t> mData;
    uint32_t mTexHeight;
    uint32_t mTexWidth;

public:
    Texture() = default;
    Texture(Texture&& _other);
    Texture(const Texture&& _other);
    Texture& addTexturePNG(const std::string& filePath, uint32_t textHeight,
        uint32_t textWidth);
    uint32_t operator[](size_t index);
};

class TextureList {
private:
    std::vector<Texture> mData;
    std::vector<std::string> mTextureNameList;

public:
    TextureList() = default;
    TextureList(size_t size) : mData(std::vector<Texture>(size)) {};
    void addTexture(const Texture& tex_);
    void addTexture(const Texture&& tex_);
    void clearTextures();
};

uint32_t makeRGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

RGBAPixel makeRGB(uint32_t rgba8888);

std::string stripFromLeft(const std::string& str_, const std::string& toStrip);

std::vector<RGBAPixel> loadPNG(std::string_view path);

std::ostream& operator<<(std::ostream& stream, RGBAPixel pixel);

#endif