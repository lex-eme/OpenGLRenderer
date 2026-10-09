#include "Model.h"

void Model::init(const Mesh inMesh, const Texture& inTexture, const Texture& inSpecular, const Sampler inSampler) {
    transform.setPosition(glm::vec3(0.0f));
    mesh = inMesh;
    diffuseTex = inTexture;
    specularTex = inSpecular;
    sampler = inSampler;
}

void Model::draw(const ShaderProgram shader, const glm::mat4& parentTransformMatrix) const {
    const glm::mat4 matrix = parentTransformMatrix * transform.getMatrix();
    shader.setMat4Uniform("mv_matrix", matrix);
    shader.setIntUniform("samp", 0);
    sampler.bind(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTex.id);
    mesh.draw();
}

void Model::draw(const ShaderProgram shader) const {
    shader.setMat4Uniform("model_matrix", transform.getMatrix());
    shader.setIntUniform("material.diffuse", 0);
    shader.setIntUniform("material.specular", 1);
    sampler.bind(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTex.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTex.id);
    mesh.draw();
}
