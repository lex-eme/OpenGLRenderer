#pragma once

#include "Light.h"


class DirectionalLight final : public Light {
    friend LitShader;

private:
    glm::vec3 direction{0.0f, -1.0f, 0.0};

public:
    void drawDebug() override;
};
