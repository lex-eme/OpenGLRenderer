#pragma once
#include "Material.h"
#include "../Texture.h"


class BatteryMaterial final : public Material {
    Texture albedo{};
    Texture metalRoughEmissive{};
    glm::vec3 chargeColor{0.1f, 0.9f, 0.1f};
    float charge{1.0f};

public:
    void bind() const override;
    void create(ShaderProgram inShader, Texture inAlbedo, Texture inMetalRoughEmissive);
    void drawDebug() override;
};
