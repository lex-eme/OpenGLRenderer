#include "Image.h"

#include <iostream>

#include "stb_image.h"

Image::~Image() {
    stbi_image_free(data);
}

bool Image::loadFromFile(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        return true;
    }

    std::cerr << "Failed to load image from file: " << path << std::endl;
    return false;
}
