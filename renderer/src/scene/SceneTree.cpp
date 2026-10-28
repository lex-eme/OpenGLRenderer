#include "../pch.h"

#include "SceneTree.h"

void saveString(FILE* file, const std::string& str) {
    const uint64_t size = str.size();
    fwrite(&size, sizeof(size), 1, file);

    if (size > 0) {
        fwrite(str.data(), sizeof(char), size, file);
    }
}

std::string loadString(FILE* file) {
    uint64_t size = 0;
    fread(&size, sizeof(size), 1, file);

    std::string str;
    if (size > 0) {
        str.resize(size);
        fread(&str[0], sizeof(char), size, file);
    }

    return str;
}

void saveStringList(FILE* file, const std::vector<std::string>& vec) {
    const uint64_t vec_size = vec.size();
    fwrite(&vec_size, sizeof(vec_size), 1, file);

    for (const auto& str: vec) {
        saveString(file, str);
    }
}

void loadStringList(FILE* file, std::vector<std::string>& vec) {
    uint64_t vec_size = 0;
    fread(&vec_size, sizeof(vec_size), 1, file);

    vec.reserve(vec_size);
    for (uint64_t i = 0; i < vec_size; ++i) {
        vec.push_back(loadString(file));
    }
}

void saveMap(FILE* f, const std::unordered_map<uint32_t, uint32_t>& map) {
    std::vector<uint32_t> ms;
    ms.reserve(map.size() * 2);
    for (const auto& m: map) {
        ms.push_back(m.first);
        ms.push_back(m.second);
    }
    const uint32_t size = static_cast<uint32_t>(ms.size());
    fwrite(&size, sizeof(size), 1, f);
    fwrite(ms.data(), sizeof(int), ms.size(), f);
}

void loadMap(FILE* file, std::unordered_map<uint32_t, uint32_t>& map) {
    std::vector<uint32_t> ms;

    uint32_t size = 0;
    fread(&size, 1, sizeof(size), file);

    ms.resize(size);
    fread(ms.data(), sizeof(int), size, file);
    for (size_t i = 0; i < (size / 2); i++)
        map[ms[i * 2 + 0]] = ms[i * 2 + 1];
}

int SceneTree::addNode(const int parent, int level) {
    const int node = static_cast<int>(hierarchy.size());

    // TODO: resize aux arrays (local/global etc.)
    localTransforms.push_back(glm::mat4(1.0f));
    globalTransforms.push_back(glm::mat4(1.0f));

    hierarchy.push_back({.parent = parent, .lastSibling = -1});

    if (parent > -1) {
        // search first sibling
        if (const int sibling = hierarchy[parent].firstChild; sibling == -1) {
            hierarchy[parent].firstChild = node;
            hierarchy[node].lastSibling = node;
        } else {
            int dest = hierarchy[sibling].lastSibling;
            if (dest <= -1) {
                // search last sibling
                for (dest = sibling; hierarchy[dest].nextSibling != -1; dest = hierarchy[dest].nextSibling);
            }
            hierarchy[dest].nextSibling = node;
            hierarchy[sibling].lastSibling = node;
        }
    }

    hierarchy[node].level = level;
    hierarchy[node].nextSibling = -1;
    hierarchy[node].firstChild = -1;
    return node;
}

void SceneTree::loadScene(const std::string& fileName) {
    FILE* file = fopen(fileName.c_str(), "rb");

    if (!file) {
        printf("Cannot open scene file '%s'. Please run SceneConverter from Chapter7 and/or MergeMeshes from Chapter 9",
               fileName.c_str());
        return;
    }

    uint32_t size = 0;
    fread(&size, sizeof(size), 1, file);

    localTransforms.resize(size);
    globalTransforms.resize(size);
    hierarchy.resize(size);

    // TODO: check > -1
    // TODO: recalculate changedAtThisLevel() - find max depth of a node [or save scene.maxLevel]
    fread(localTransforms.data(), sizeof(glm::mat4), size, file);
    fread(globalTransforms.data(), sizeof(glm::mat4), size, file);
    fread(hierarchy.data(), sizeof(Hierarchy), size, file);

    loadMap(file, materialForNode);
    loadMap(file, meshes);

    if (!feof(file)) {
        loadMap(file, nameForNode);
        loadStringList(file, names);
        loadStringList(file, materialNames);
    }
    fclose(file);
}

void SceneTree::saveScene(const std::string& fileName) const {
    FILE* file = fopen(fileName.c_str(), "wb");

    const uint32_t size = static_cast<uint32_t>(hierarchy.size());
    fwrite(&size, sizeof(size), 1, file);

    fwrite(localTransforms.data(), sizeof(glm::mat4), size, file);
    fwrite(globalTransforms.data(), sizeof(glm::mat4), size, file);
    fwrite(hierarchy.data(), sizeof(Hierarchy), size, file);

    saveMap(file, materialForNode);
    saveMap(file, meshes);

    if (!names.empty() && !nameForNode.empty()) {
        saveMap(file, nameForNode);
        saveStringList(file, names);
        saveStringList(file, materialNames);
    }

    fclose(file);
}

void SceneTree::markAsChanged(const int node) {
    int level = hierarchy[node].level;
    changedAtThisFrame[level].push_back(node);

    // TODO: use non-recursive iteration with aux stack
    for (int s = hierarchy[node].firstChild; s != -1; s = hierarchy[s].nextSibling) {
        markAsChanged(s);
    }
}

void SceneTree::recalculateGlobalTransforms() {
    if (!changedAtThisFrame[0].empty()) {
        int c = changedAtThisFrame[0][0];
        globalTransforms[c] = localTransforms[c];
        changedAtThisFrame[0].clear();
    }

    for (int i = 1; i < MAX_NODE_LEVEL && !changedAtThisFrame[i].empty(); i += 1) {
        for (const int child: changedAtThisFrame[i]) {
            const int parent = hierarchy[child].parent;
            globalTransforms[child] = globalTransforms[parent] * localTransforms[child];
        }
        changedAtThisFrame[i].clear();
    }
}
