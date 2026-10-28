#pragma once
#include <vector>

#include "glm/fwd.hpp"

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
    std::vector<glm::mat4> localTransforms;
    std::vector<glm::mat4> globalTransforms;
    std::vector<int> changedAtThisFrame[MAX_NODE_LEVEL];
    std::vector<Hierarchy> hierarchy;
    std::unordered_map<unsigned int, unsigned int> meshes; // node -> mesh
    std::unordered_map<unsigned int, unsigned int> materialForNode; // node -> material
    std::unordered_map<unsigned int, unsigned int> nameForNode; // node -> name
    std::vector<std::string> names;
    std::vector<std::string> materialNames;

    int addNode(int parent, int level);
    void loadScene(const std::string& fileName);
    void saveScene(const std::string& fileName) const;

    void markAsChanged(int node);
    void recalculateGlobalTransforms();
};
