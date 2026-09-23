#pragma once
#include <glad/glad.h>

#include "ShaderProgram.h"
#include "Transform.h"


class Mesh {
public:
    Transform transform{};
    GLuint vbo{};

private:
    GLsizei vertexCount{};

public:
    void create(GLsizei vCount, GLsizeiptr size, const void* data);
    void draw(ShaderProgram shader, const glm::mat4& parentTransform) const;
};
