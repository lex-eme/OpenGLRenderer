#include "GridMaterial.h"

#include "../Camera.h"

GridMaterial::GridMaterial()
    : Material() {
}

void GridMaterial::bind() const {
    Material::bind();
    shader.setVec3Uniform("cameraPos", camera->position);
}

void GridMaterial::create(const ShaderProgram inShader, Camera* inCamera) {
    shader = inShader;
    camera = inCamera;
}

void GridMaterial::drawDebug() {
}
