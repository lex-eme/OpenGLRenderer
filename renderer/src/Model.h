#pragma once

#include "material/Material.h"
#include "mesh/Mesh.h"
#include "Transform.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

class Model {
public:
    Transform transform;
    std::vector<Material*> materials;

private:
    std::vector<Mesh> meshes;

public:
    Model() = default;
    explicit Model(const std::string& filePath);

    void draw();
    void drawDebug(int id);

private:
    void loadModel(const std::string& path);
    void processNode(const aiNode* node, const aiScene* scene);
    static Mesh processMesh(const aiMesh* mesh);
};
