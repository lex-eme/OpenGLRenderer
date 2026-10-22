#pragma once
#include <unordered_map>

#include "Texture.h"


class AssetManager {
    std::unordered_map<std::string, Texture> textures;

public:
    void addTexture(const std::string& name, const std::string& path);
    [[nodiscard]] const Texture& getTexture(const std::string& name) const;
};
