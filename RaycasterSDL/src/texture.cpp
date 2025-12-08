#include <Raycaster/Texture.hpp>

#include <assert.h>
#include <filesystem>
#include <iostream>


#include <SDL3_Image/SDL_image.h>

#define _ASSERT assert

void Texture::addTexturePNG(const std::string& filePath,
    uint32_t textHeight, uint32_t textWidth) {
    if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("Invalid path! " + filePath);
    }
    std::vector<RGBAPixel> pixels;
    mTexWidth = textWidth;
    mTexHeight = textHeight;

    pixels.resize(mTexHeight * mTexWidth);
    mData.resize(mTexHeight * mTexWidth);
    pixels = loadPNG(filePath);
    for (int x = 0; x < mTexWidth; x++) {
        for (int y = 0; y < mTexHeight; y++) {
            RGBAPixel texel = pixels[mTexWidth * y + x];
            mData[mTexWidth * y + x] =
                makeRGBA8888(texel.r, texel.g, texel.b, texel.a);
        }
    }

};

uint32_t Texture::operator[](size_t index) {
    _ASSERT(index < mTexHeight * mTexWidth && index >= 0);
    return mData[index];
}

uint32_t makeRGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}

RGBAPixel makeRGBA8888(uint32_t rgba8888) {
    uint8_t r = rgba8888 >> 24;
    uint8_t g = (rgba8888 << 8) >> 24;
    uint8_t b = (rgba8888 << 16) >> 24;
    uint8_t a = (rgba8888 << 24) >> 24;
    return { r, g, b, a };
};

std::string stripFromLeft(const std::string& str_, const std::string& toStrip) {
    size_t indexToStrip;
    for (size_t i = 0; i < str_.size(); i++) {
        if (toStrip[i] != str_[i])
            return str_;
        if (i == toStrip.size() - 1)
            indexToStrip = i + 1;
    }
    return std::string(str_.begin() + indexToStrip, str_.end());
};

std::vector<RGBAPixel> loadPNG(std::string_view path) {
    SDL_Surface* sur = IMG_Load(path.data());

    SDL_Surface* rgbaSurface = SDL_ConvertSurface(sur, SDL_PIXELFORMAT_RGBA8888);
    if (!rgbaSurface) {
        std::cerr << "Failed to create surface " << SDL_GetError() << std::endl;
        return std::vector<RGBAPixel>();
    }
    SDL_DestroySurface(sur);

    int width = rgbaSurface->w;
    int height = rgbaSurface->h;
    std::vector<RGBAPixel> pixels(width * height);

    uint8_t* pixelData = static_cast<uint8_t*>(rgbaSurface->pixels);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int srcIndex = (y * rgbaSurface->pitch) + (x * 4);
            int dstIndex = y * width + x;
            pixels[dstIndex].r = pixelData[srcIndex + 3];
            pixels[dstIndex].g = pixelData[srcIndex + 2];
            pixels[dstIndex].b = pixelData[srcIndex + 1];
            pixels[dstIndex].a = pixelData[srcIndex];
        }
    }

    SDL_DestroySurface(rgbaSurface);
    return pixels;
}

std::ostream& operator<<(std::ostream& stream, RGBAPixel pixel) {
    stream << " R: " << (int)pixel.r << \
        " G: " << (int)pixel.g \
        << " B: " << (int)pixel.b \
        << " A: " << (int)pixel.a << std::endl;
    return stream;
}