#include "../pch.h"

#include "Grid.h"


void Grid::create(Camera* camera, const UniformBufferObject ubo) {
    const ShaderProgram gridShader = ShaderProgram::createShaderProgram(
        "assets/shaders/grid.vert", "assets/shaders/grid.frag");
    gridShader.bindUniformBufferObject("Matrices", ubo.bindingPoint);
    material.create(gridShader, camera);
    verticeCount = 6;

    glGenVertexArrays(1, &vao);
}

void Grid::draw() const {
    material.bind();
    glDisable(GL_CULL_FACE);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool Grid::checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        std::cerr << "glError: " << glErr << std::endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}