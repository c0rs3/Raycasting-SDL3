#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void loadPNG(const std::string& filename) {
    int width, height, channels;

    uint8_t* imageData = stbi_load(filename.c_str(), &width, &height, &channels, 3);
    if (imageData == nullptr) {
        std::cerr << "Error loading image: " << filename << std::endl;
        return;
    }
    std::vector<RGBPixel> pixels(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            RGBPixel& pixel = pixels[y * width + x];

            pixel.red = imageData[index];
            pixel.green = imageData[index + 1];
            pixel.blue = imageData[index + 2];
        }
    }

    for (int y = 0; y < 10 && y < height; ++y) {
        for (int x = 0; x < 10 && x < width; ++x) {
            RGBPixel& pixel = pixels[y * width + x];
            std::cout << "Pixel at (" << x << "," << y << ") - R: " 
            << (int)pixel.red << " G: " << (int)pixel.green << " B: " << (int)pixel.blue << std::endl;
        }
    }
    stbi_image_free(imageData);
}


