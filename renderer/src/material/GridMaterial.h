#pragma once
#include "Material.h"


class Camera;

class GridMaterial final : public Material {
    Camera* camera{};

public:
    GridMaterial();

    void bind() const override;
    void create(ShaderProgram inShader, Camera* inCamera);
    void drawDebug() override;
};
