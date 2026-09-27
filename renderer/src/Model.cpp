#include "Model.h"

void Model::init(const Mesh inMesh, const GLuint inTextureID) {
    transform.setPosition(glm::vec3(0.0f));
    mesh = inMesh;
    textureID = inTextureID;
}

void Model::draw(const ShaderProgram shader, const glm::mat4& parentTransformMatrix) const {
    const glm::mat4 matrix = parentTransformMatrix * transform.getMatrix();
    shader.setMat4Uniform("mv_matrix", matrix);
    shader.setIntUniform("samp", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    mesh.draw();
}
