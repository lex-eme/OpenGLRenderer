#pragma once
#include "Material.h"


class Camera;

class GridMaterial final : public Material {
    Camera* camera{};

public:
    explicit GridMaterial(Camera* inCamera);

    void bind() const override;
    void create(ShaderProgram inShader);
    void drawDebug() override;
};
