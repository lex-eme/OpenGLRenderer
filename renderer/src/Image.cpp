#include "Image.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::~Image() {
    stbi_image_free(data);
}

bool Image::loadFromFile(const std::string& path) {
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        return true;
    }

    std::cerr << "Failed to load image from file: " << path << std::endl;
    return false;
}
