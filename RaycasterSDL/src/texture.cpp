#include <Raycaster.hpp>
#include <Raycaster/Texture.hpp>

uint32_t makeRGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}

std::string stripString(const std::string& iString, const std::string& ToStrip) {
    size_t indexToStrip;
    for (size_t i = 0; i < ToStrip.size(); i++) {
        if (ToStrip[i] != iString[i])
            return iString;
        if (i == ToStrip.size() - 1)
            indexToStrip = i + 1;
    }
    return std::string(iString.begin() + indexToStrip, iString.end());
}

std::vector<RGBPixel> loadPNG(const std::string& filename) {
    SDL_Surface* sur = IMG_Load(filename.c_str());
    
    SDL_Surface* rgbaSurface = SDL_ConvertSurface(sur, SDL_PIXELFORMAT_RGBA8888);
    if (!rgbaSurface) {
        std::cerr << "Failed to create surface " << SDL_GetError() << std::endl;
        return std::vector<RGBPixel>();
    }
    SDL_DestroySurface(sur);

    int width = rgbaSurface->w;
    int height = rgbaSurface->h;
    std::vector<RGBPixel> pixels(width * height);

    uint8_t* pixelData = static_cast<uint8_t*>(rgbaSurface->pixels);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int srcIndex = (y * rgbaSurface->pitch) + (x * 4);
            int dstIndex = y * width + x;
            // pixelData[srcIndex + 0] is the alpha value
            // pixels[dstIndex].a = pixelData[srcIndex + 0];
            pixels[dstIndex].g = pixelData[srcIndex + 1];
            pixels[dstIndex].b = pixelData[srcIndex + 2];
            pixels[dstIndex].r = pixelData[srcIndex + 3];
        }
    }

    SDL_DestroySurface(rgbaSurface);
    return pixels;
}

std::ostream& operator<<(std::ostream& stream, RGBPixel pixel) {
    stream << " R: " << (int)pixel.r
        << " G: " << (int)pixel.g
        << " B: " << (int)pixel.b << std::endl;
    return stream;
}

Texture::Texture() = default;
Texture::~Texture() = default;

uint32_t Texture::operator[](size_t index) {
    return mData[index];
}

void Texture::addTexturePNG(const std::string& filePath, unsigned int texHeight,
    unsigned int texWidth) {
    textureNameList.push_back(stripString(filePath, TEXTURE_ASSET_PATH));
    if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("Invalid path! " + filePath);
    }
    std::vector<RGBPixel> pixels;
    pixels.resize(texHeight * texWidth);
    mData.resize(texHeight * texWidth);

    pixels = loadPNG(filePath);
    for (int x = 0; x < texWidth; x++) {
        for (int y = 0; y < texHeight; y++) {
            RGBPixel texel = pixels[texWidth * y + x];
            mData[texWidth * y + x] = makeRGBA8888(texel.r, texel.b, texel.g);
        }
    }
};

void Texture::clearTextures() {
    mData.clear();
}