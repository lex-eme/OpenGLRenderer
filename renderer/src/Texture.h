#pragma once
#include <string>

class Texture final {
public:
    unsigned int id = 0;

public:
    bool loadFromFile(const std::string& inPath);
    void unload();
};
