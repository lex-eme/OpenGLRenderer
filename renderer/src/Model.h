#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Transform.h"


class Model {
public:
    Transform transform;

private:
    Mesh mesh;
    GLuint textureID{};

public:
    void init(Mesh inMesh, GLuint inTextureID);
    void draw(ShaderProgram shader, const glm::mat4& parentTransformMatrix) const;
};
