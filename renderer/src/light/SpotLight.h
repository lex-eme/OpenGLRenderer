#pragma once
#include "Light.h"


class SpotLight final : public Light {
    friend LitShader;

private:
    glm::vec3 position{};
    glm::vec3 direction{0.0f, -1.0f, 0.0};
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    float innerCutOff = 12.5f;
    float outerCutOff = 15.0f;

public:
    void drawDebug() override;
};
