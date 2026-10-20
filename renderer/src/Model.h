#pragma once

#include "mesh/Mesh.h"
#include "shader/ShaderProgram.h"
#include "Texture.h"
#include "Transform.h"
#include "assimp/material.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

class Model {
public:
    Transform transform;

private:
    std::vector<Mesh> meshes;
    std::vector<Texture> loadedTextures;
    std::string directory;

public:
    Model() = default;
    explicit Model(const std::string& filePath);

    void draw(ShaderProgram shader);

private:
    void loadModel(const std::string& path);
    void processNode(const aiNode* node, const aiScene* scene);
    Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName);
};
