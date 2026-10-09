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
    Texture diffuseTex;
    Texture specularTex;
    Sampler sampler;

public:
    void init(Mesh inMesh, const Texture& inTexture, const Texture& inSpecular = Texture(),
              Sampler inSampler = Sampler());
    void draw(ShaderProgram shader, const glm::mat4& parentTransformMatrix) const;
    void draw(ShaderProgram shader) const;
};
