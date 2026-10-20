#pragma once
#include <string>

class Texture final {
public:
    unsigned int id = 0;
    std::string type;
    std::string path;

public:
    void loadFromFile(const std::string& inPath);
    void unload();
};
