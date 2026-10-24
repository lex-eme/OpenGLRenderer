#include "AssetManager.h"

#include <iostream>
#include <ranges>

#include "material/Material.h"

AssetManager::~AssetManager() {
    for (auto& texture: textures | std::views::values) {
        texture.unload();
    }

    for (const auto& material: materials | std::views::values) {
        delete material;
    }
}

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

void AssetManager::addMaterial(const std::string& name, Material* material) {
    if (materials.contains(name)) {
        std::cout << "Material '" << name << "' already exists" << std::endl;
        return;
    }

    materials[name] = material;
}

const Material* AssetManager::getMaterial(const std::string& name) const {
    if (const auto it = materials.find(name); it != materials.end()) {
        return it->second;
    }

    std::cerr << "Trying to use unknown material called " << name << std::endl;
    exit(-1);
}

const std::unordered_map<std::string, Material*>& AssetManager::getMaterialMap() const {
    return materials;
}

const Texture& AssetManager::getTexture(const std::string& name) const {
    if (const auto it = textures.find(name); it != textures.end()) {
        return it->second;
    }

    std::cerr << "Trying to use unknown texture called " << name << std::endl;
    exit(-1);
}
