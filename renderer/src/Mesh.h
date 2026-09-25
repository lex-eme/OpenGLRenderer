#pragma once
#include <glad/glad.h>

#include "ShaderProgram.h"
#include "Transform.h"


class Mesh {
public:
    Transform transform{};

private:
    GLuint vbo[2]{};
    GLuint textureID{};
    GLsizei vertexCount{};

public:
    void create(GLsizei vCount, GLsizeiptr vSize, const void* vData, GLsizeiptr tSize, const void* tData, GLuint texID);
    void draw(ShaderProgram shader, const glm::mat4& parentTransform) const;
};
