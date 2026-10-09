#pragma once
#include <string>

#include "Sampler.h"

class Texture final {
public:
    unsigned int id = 0;

public:
    void loadFromFile(const std::string& path);
    void unload();
};
