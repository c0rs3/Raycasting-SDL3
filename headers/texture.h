#ifndef RAYCASTER_TEXTURE
#define RAYCASTER_TEXTURE

#include "raycasterutil.h"

struct RGBPixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void loadPNG(const std::string& filename);

#endif