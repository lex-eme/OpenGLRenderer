#include "ColorMaterial.h"

void ColorMaterial::bind() const {
    Material::bind();
    shader.setVec3Uniform("inColor", color);
}

void ColorMaterial::create(const ShaderProgram inShader, const glm::vec3 inColor) {
    shader = inShader;
    color = inColor;
}