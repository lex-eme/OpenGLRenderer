#include "Grid.h"

#include <iostream>
#include <glad/glad.h>

#include "../material/Material.h"


void Grid::create(Material* inMaterial) {
    material = inMaterial;
    verticeCount = 6;

    glGenVertexArrays(1, &vao);
}

void Grid::draw() const {
    material->bind();
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