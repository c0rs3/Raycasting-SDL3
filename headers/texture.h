#ifndef RAYCASTER_TEXTURE
#define RAYCASTER_TEXTURE

#include "raycasterutil.h"

uint32_t makeRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
struct RGBPixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class Texture {
public:
    Texture();
    ~Texture();
    void addTexturePNG(const std::string& filePath, unsigned int textHeight, unsigned int textWidth);
public:
    unsigned int mHeight;
    unsigned int mWidth;
    std::vector<uint32_t> mData;
private:
};


std::vector<RGBPixel> loadPNG(const std::string& filename);

std::ostream& operator<<(std::ostream& stream, RGBPixel pixel);
#endif