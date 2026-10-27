#include "GridMaterial.h"

#include "../Camera.h"

GridMaterial::GridMaterial(Camera* inCamera)
    : Material(), camera(inCamera) {
}

void GridMaterial::bind() const {
    Material::bind();
    shader.setVec3Uniform("cameraPos", camera->position);
}

void GridMaterial::create(const ShaderProgram inShader) {
    shader = inShader;
}

void GridMaterial::drawDebug() {
}
