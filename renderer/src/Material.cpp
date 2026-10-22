#include "Material.h"

Material::Material(const AssetManager& manager) {
    shader = ShaderProgram::createShaderProgram("assets/shaders/unlit.vert", "assets/shaders/unlit.frag");
    diffuse = manager.getTexture("backpack_diffuse");
}

void Material::bind() const {
    shader.setActive();
    shader.setIntUniform("material.texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse.id);
}
