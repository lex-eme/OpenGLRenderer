#include "Grid.h"

#include <iostream>
#include <glad/glad.h>

#include "../material/Material.h"


void Grid::create(Material* inMaterial) {
    material = inMaterial;
    verticeCount = 6;

    float planeVertices[] = {
        -1.0f, 0.0f, -1.0f,
         1.0f, 0.0f, -1.0f,
         1.0f, 0.0f,  1.0f,
         1.0f, 0.0f,  1.0f,
        -1.0f, 0.0f,  1.0f,
        -1.0f, 0.0f, -1.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void Grid::draw() const {
    material->bind();
    glDisable(GL_CULL_FACE);
    glBindVertexArray(vao);
    checkOpenGLError();

    glDrawArrays(GL_TRIANGLES, 0, verticeCount);
    checkOpenGLError();

    glBindVertexArray(0);
    checkOpenGLError();
    glActiveTexture(GL_TEXTURE0);
    checkOpenGLError();
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