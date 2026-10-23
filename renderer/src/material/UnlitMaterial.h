#pragma once
#include "Material.h"

#include "../Texture.h"


class UnlitMaterial final : public Material {
    Texture diffuse{};

public:
    UnlitMaterial() = default;

    void bind() const override;
    void create(ShaderProgram inShader, Texture inDiffuse);
};
