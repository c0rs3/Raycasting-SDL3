#include "texture.h"

// STB Preprocessor must be kept here or multiple definitions will occur
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

uint32_t makeRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}

std::vector<RGBPixel> loadPNG(const std::string& filename) {
    int width, height, channels;

    uint8_t* imageData = stbi_load(filename.c_str(), &width, &height, &channels, 3);
    if (imageData == nullptr) {
        std::cerr << "Error loading image: " << filename << std::endl;
        return std::vector<RGBPixel>();
    }
    std::vector<RGBPixel> pixels(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            RGBPixel& pixel = pixels[y * width + x];

            pixel.r = imageData[index];
            pixel.g = imageData[index + 1];
            pixel.b = imageData[index + 2];
        }
    }
    /*
    for (int y = 0; y < 10 && y < height; ++y) {
        for (int x = 0; x < 10 && x < width; ++x) {
            RGBPixel& pixel = pixels[y * width + x];
            std::cout << "Pixel at (" << x << "," << y << ") - R: "
            << (int)pixel.red << " G: " << (int)pixel.green << " B: " << (int)pixel.blue << std::endl;
        }
    }
    */
    stbi_image_free(imageData);
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

void Texture::addTexturePNG(const std::string& filePath, unsigned int texHeight, unsigned int texWidth) {
    if (!std::filesystem::exists(filePath)){
        throw std::runtime_error("Invalid path! " + filePath);
    }
    std::vector<RGBPixel> pixels;
    pixels.resize(texHeight * texWidth);
    mData.resize(texHeight * texWidth);
    pixels = loadPNG(filePath);
    for (int x = 0; x < texWidth; x++) {
        for (int y = 0; y < texHeight; y++) {
            RGBPixel texel = pixels[texWidth * y + x];
            mData[texWidth * y + x] = makeRGBA(texel.r, texel.b, texel.g);
        }
    }
};
