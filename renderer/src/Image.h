#pragma once
#include <string>


class Skybox;
class Texture;

class Image final {
    friend Texture;
    friend Skybox;

private:
    int width{}, height{}, nrChannels{};
    unsigned char* data{};

public:
    Image() = default;
    ~Image();

    bool loadFromFile(const std::string& path);
};
