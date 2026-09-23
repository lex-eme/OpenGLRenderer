#include "Mesh.h"

void Mesh::create(const GLsizei vCount, const GLsizeiptr size, const void* data) {
    vertexCount = vCount;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Mesh::draw(const ShaderProgram shader, const glm::mat4& parentTransform) const {
    const glm::mat4 matrix = parentTransform * transform.getMatrix();
    shader.setMat4Uniform("mv_matrix", matrix);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
