#include "AssetManager.h"

#include <iostream>

void AssetManager::addTexture(const std::string& name, const std::string& path) {
    if (textures.contains(name)) {
        std::cout << "Texture '" << name << "' already exists" << std::endl;
        return;
    }

    if (!textures[name].loadFromFile(path)) {
        std::cerr << "Could not load texture '" << name << "' from path '" << path << "'" << std::endl;
        textures.erase(name);
    } else {
        std::cout << "Loaded Texture '" << name << "' from path '" << path << "'" << std::endl;
    }
}

const Texture& AssetManager::getTexture(const std::string& name) const {
    if (const auto it = textures.find(name); it != textures.end()) {
        return it->second;
    }

    std::cerr << "Trying to use unknown texture called " << name << std::endl;
    exit(-1);
}
