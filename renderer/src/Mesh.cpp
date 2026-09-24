#include "Mesh.h"

void Mesh::create(const GLsizei vCount, const GLsizeiptr vSize, const void* vData, const GLsizeiptr tSize,
                  const void* tData, const GLuint texID) {
    vertexCount = vCount;
    textureID = texID;
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vSize, vData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tSize, tData, GL_STATIC_DRAW);
}

void Mesh::draw(const ShaderProgram shader, const glm::mat4& parentTransform) const {
    const glm::mat4 matrix = parentTransform * transform.getMatrix();
    shader.setMat4Uniform("mv_matrix", matrix);
    shader.setIntUniform("samp", 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CCW);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
