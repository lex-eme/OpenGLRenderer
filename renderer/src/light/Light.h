#pragma once

#include "glm/vec3.hpp"
#include "../mesh/Mesh.h"

class LitShader;

class Light {
    friend LitShader;

protected:
    glm::vec3 ambient{0.1f, 0.1f, 0.1f};
    glm::vec3 diffuse{0.5f, 0.5f, 0.5f};
    glm::vec3 specular{1.0f, 1.0f, 1.0f};

public:
    Light() = default;
    Light(const glm::vec3& inAmbient, const glm::vec3& inDiffuse, const glm::vec3& inSpecular);
    virtual ~Light() = default;

    virtual void drawDebug();
};
