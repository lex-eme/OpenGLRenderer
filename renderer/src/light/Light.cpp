#include "Light.h"


Light::Light(const glm::vec3& inAmbient, const glm::vec3& inDiffuse, const glm::vec3& inSpecular)
    : ambient(inAmbient), diffuse(inDiffuse), specular(inSpecular) {
}

void Light::drawDebug() {
}
