#include "UnlitMaterial.h"

#include <glad/glad.h>


void UnlitMaterial::bind() const {
    Material::bind();
    shader.setIntUniform("material.texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse.id);
}

void UnlitMaterial::create(const ShaderProgram inShader, const Texture inDiffuse) {
    shader = inShader;
    diffuse = inDiffuse;
}
