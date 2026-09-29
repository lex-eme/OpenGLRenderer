#pragma once
#include "mesh/Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Transform.h"


class Model {
public:
    Transform transform;

private:
    Mesh mesh;
    Texture texture;

public:
    void init(Mesh inMesh, const Texture& inTexture);
    void draw(ShaderProgram shader, const glm::mat4& parentTransformMatrix) const;
};
