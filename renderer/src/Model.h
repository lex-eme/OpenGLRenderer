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
    std::vector<std::string> materialNames;

private:
    std::vector<Mesh> meshes;

public:
    Model() = default;
    explicit Model(const std::string& filePath, bool flipUvs = true);

    void draw();
    void drawDebug();

private:
    void loadModel(const std::string& path, bool flipUvs = true);
    void processNode(const aiNode* node, const aiScene* scene);
    static Mesh processMesh(const aiMesh* mesh);
};
