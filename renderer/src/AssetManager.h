#pragma once
#include <unordered_map>

#include "Texture.h"


class Material;

class AssetManager {
    std::unordered_map<std::string, Texture> textures;
    std::unordered_map<std::string, Material*> materials;

public:
    ~AssetManager();

    void addTexture(const std::string& name, const std::string& path);
    void addMaterial(const std::string& name, Material* material);

    [[nodiscard]] const Texture& getTexture(const std::string& name) const;
    [[nodiscard]] const Material* getMaterial(const std::string& name) const;

    [[nodiscard]] const std::unordered_map<std::string, Material*>& getMaterialMap() const;
};
