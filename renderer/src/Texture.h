#pragma once
#include <string>

// TODO: when deallocate from GPU?
class Texture final {
public:
    unsigned int textureID = 0;

public:
    void loadFromFile(const std::string& path);
};
