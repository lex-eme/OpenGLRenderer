#pragma once
#include <string>


class Texture;

class Image final {
    friend Texture;

private:
    int width, height, nrChannels;
    unsigned char* data;

public:
    Image() = default;
    ~Image();

    bool loadFromFile(const std::string& path);
};
