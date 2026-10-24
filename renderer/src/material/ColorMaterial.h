#pragma once
#include "Material.h"


class ColorMaterial final : public Material {
    glm::vec3 color{1.0f};

public:
    ColorMaterial() = default;

    void bind() const override;
    void create(ShaderProgram inShader, glm::vec3 inColor);
    void drawDebug() override;
};
