#pragma once
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

#include "../mesh/Mesh.h"

class Material;
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

constexpr int MAX_NODE_LEVEL = 16;

// Left child - right sibling binary tree
struct Hierarchy {
    int parent;
    int firstChild;
    int nextSibling;
    int lastSibling;
    int level; // depth from root node
};

class SceneTree {
public:
    std::vector<Material*> materials;

private:
    std::vector<glm::mat4> localTransforms;
    std::vector<glm::mat4> globalTransforms;
    std::vector<int> changedAtThisFrame[MAX_NODE_LEVEL];
    std::vector<Hierarchy> hierarchy;
    std::unordered_map<unsigned int, unsigned int> meshMap; // node -> mesh
    std::unordered_map<unsigned int, unsigned int> materialForNode; // node -> material
    std::unordered_map<unsigned int, unsigned int> nameForNode; // node -> name
    std::vector<std::string> names;
    std::vector<std::string> materialNames;

    std::vector<Mesh> meshes;

public:
    void loadScene(const std::string& fileName);
    void saveScene(const std::string& fileName) const;
    void loadModel(const std::string& path, bool flipUvs);

    void draw();
    void drawDebug();

private:
    int addNode(int parent, int level);
    void markAsChanged(int node);
    void recalculateGlobalTransforms();

    void processNode(const aiNode* node, const aiScene* scene, int parent, int level);
    static Mesh processMesh(const aiMesh* mesh);

    int drawSceneTree(int node);
};
