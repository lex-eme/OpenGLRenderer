#pragma once
#include "Light.h"


class PointLight final : public Light {
    friend LitShader;

private:
    glm::vec3 position{};
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

public:
    void drawDebug() override;
};
