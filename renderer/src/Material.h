#pragma once
#include "AssetManager.h"
#include "Texture.h"
#include "shader/ShaderProgram.h"


struct Material {
    ShaderProgram shader{};
    Texture diffuse{};

public:
    Material() = default;
    explicit Material(const AssetManager& manager);

    void bind() const;
};
