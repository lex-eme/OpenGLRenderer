#define GLM_ENABLE_EXPERIMENTAL

#include "../pch.h"

#include "SceneTree.h"

#include "Scene.h"
#include "../Transform.h"
#include "../material/ColorMaterial.h"
#include "../material/Material.h"
#include "../material/UnlitMaterial.h"
#include "glm/gtx/string_cast.hpp"

void printMat4(const glm::mat4& mat) {
    std::cout << "[" << mat[0][0] << "|" << mat[0][1] << "|" << mat[0][2] << "|" << mat[0][3] << "]\n";
    std::cout << "[" << mat[1][0] << "|" << mat[1][1] << "|" << mat[1][2] << "|" << mat[1][3] << "]\n";
    std::cout << "[" << mat[2][0] << "|" << mat[2][1] << "|" << mat[2][2] << "|" << mat[2][3] << "]\n";
    std::cout << "[" << mat[3][0] << "|" << mat[3][1] << "|" << mat[3][2] << "|" << mat[3][3] << "]\n";
}

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

int SceneTree::addNode(const int parent, const int level) {
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
    loadMap(file, meshMap);

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
    saveMap(file, meshMap);

    if (!names.empty() && !nameForNode.empty()) {
        saveMap(file, nameForNode);
        saveStringList(file, names);
        saveStringList(file, materialNames);
    }

    fclose(file);
}

void SceneTree::loadModel(const std::string& path, const bool flipUvs) {
    unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
    flags |= flipUvs ? aiProcess_FlipUVs : 0;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    std::string directory = path.substr(0, path.find_last_of('/'));
    const ShaderProgram unlitShader = ShaderProgram::createShaderProgram(
        "assets/shaders/unlit.vert", "assets/shaders/unlit.frag");
    const ShaderProgram colorShader = ShaderProgram::createShaderProgram(
        "assets/shaders/singleColor.vert", "assets/shaders/singleColor.frag");

    materials.reserve(scene->mNumMaterials);
    for (int matIndex = 0; matIndex < scene->mNumMaterials; matIndex += 1) {
        const aiMaterial* material = scene->mMaterials[matIndex];
        auto mat = Scene::assetManager.getMaterial("M_Default");
        materials.push_back(mat);
        materialNames.emplace_back(material->GetName().C_Str());

        std::cout << "material name: " << materialNames.back() << std::endl;
        //material->Get()

        bool hasTexture = false;
        for (int texType = 0; texType <= 17; texType += 1) {
            const unsigned int texCount = material->GetTextureCount(static_cast<aiTextureType>(texType));

            for (int texId = 0; texId < texCount; texId += 1) {
                aiString str;
                if (material->GetTexture(static_cast<aiTextureType>(texType), texId, &str) == aiReturn_SUCCESS) {
                    std::string texFullPath(str.C_Str());
                    std::filesystem::path p(texFullPath);
                    std::cout << "texture | mat index: " << matIndex << " | type: " << texType << " | name: " << p.
                            filename() << std::endl;

                    UnlitMaterial* newMat = new UnlitMaterial();
                    Texture texture;
                    texture.loadFromFile(directory + "/" + p.filename().string());
                    newMat->create(unlitShader, texture);
                    materials[matIndex] = newMat;
                    hasTexture = true;
                    break;
                }
            }
        }

        if (!hasTexture) {
            aiColor4D Color;
            // if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &Color) == AI_SUCCESS) {
            //     glm::vec3 c;
            //     c.r = Color.r;
            //     c.g = Color.g;
            //     c.b = Color.b;
            //     ColorMaterial* colorMat = new ColorMaterial();
            //     colorMat->create(colorShader, c);
            //     materials[matIndex] = colorMat;
            //     std::cout << materialNames[matIndex] << " ambient " << glm::to_string(c) << std::endl;
            // }
            if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &Color) == AI_SUCCESS) {
                glm::vec3 c;
                c.r = Color.r;
                c.g = Color.g;
                c.b = Color.b;
                ColorMaterial* colorMat = new ColorMaterial();
                colorMat->create(colorShader, c);
                materials[matIndex] = colorMat;
                std::cout << materialNames[matIndex] << " diffuse " << glm::to_string(c) << std::endl;
            }
            // if (aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &Color) == AI_SUCCESS) {
            //     glm::vec3 c;
            //     c.r = Color.r;
            //     c.g = Color.g;
            //     c.b = Color.b;
            //     ColorMaterial* colorMat = new ColorMaterial();
            //     colorMat->create(colorShader, c);
            //     materials[matIndex] = colorMat;
            //     std::cout << materialNames[matIndex] << " emissive " << glm::to_string(c) << std::endl;
            // }
        }
    }

    meshes.reserve(scene->mNumMeshes);
    for (int i = 0; i < scene->mNumMeshes; i += 1) {
        meshes.push_back(processMesh(scene->mMeshes[i]));
    }

    processNode(scene->mRootNode, scene, -1, 0);

    Transform transform;
    transform.setScale(glm::vec3(0.01f));
    localTransforms[0] = transform.getMatrix();
    markAsChanged(0);
    recalculateGlobalTransforms();
}

void SceneTree::draw() {
    for (unsigned int i = 0; i < globalTransforms.size(); i += 1) {
        if (meshMap.contains(i)) {
            const unsigned int meshIndex = meshMap[i];
            const unsigned int matIndex = materialForNode[i];

            materials[matIndex]->bind();
            materials[matIndex]->shader.setMat4Uniform("model", globalTransforms[i]);
            meshes[meshIndex].draw();
        }
    }
}

void SceneTree::drawDebug() {
    ImGui::Begin("Scene tree", nullptr);
    drawSceneTree(0);
    ImGui::End();
}

int SceneTree::drawSceneTree(const int node) {
    int selected = -1;
    const std::string name = names[node];
    const std::string label = name.empty() ? std::string("Node") + std::to_string(node) : name;

    const int flags = hierarchy[node].firstChild < 0 ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet : 0;
    const bool opened = ImGui::TreeNodeEx(&hierarchy[node], flags, "%s", label.c_str());

    ImGui::PushID(node);

    if (ImGui::IsItemClicked(0)) {
        printf("Selected node: %d (%s)\n", node, label.c_str());
        selected = node;
    }

    if (opened) {
        for (int ch = hierarchy[node].firstChild; ch != -1; ch = hierarchy[ch].nextSibling) {
            if (const int subNode = drawSceneTree(ch); subNode > -1)
                selected = subNode;
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
    return selected;
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

glm::mat4 toMat4(const aiMatrix4x4& from) {
    glm::mat4 to;
    to[0][0] = static_cast<float>(from.a1);
    to[0][1] = static_cast<float>(from.b1);
    to[0][2] = static_cast<float>(from.c1);
    to[0][3] = static_cast<float>(from.d1);
    to[1][0] = static_cast<float>(from.a2);
    to[1][1] = static_cast<float>(from.b2);
    to[1][2] = static_cast<float>(from.c2);
    to[1][3] = static_cast<float>(from.d2);
    to[2][0] = static_cast<float>(from.a3);
    to[2][1] = static_cast<float>(from.b3);
    to[2][2] = static_cast<float>(from.c3);
    to[2][3] = static_cast<float>(from.d3);
    to[3][0] = static_cast<float>(from.a4);
    to[3][1] = static_cast<float>(from.b4);
    to[3][2] = static_cast<float>(from.c4);
    to[3][3] = static_cast<float>(from.d4);
    return to;
}

void SceneTree::processNode(const aiNode* node, const aiScene* scene, const int parent, const int level) {
    const int newNode = addNode(parent, level);

    if (node->mName.C_Str()) {
        const size_t nameIndex = names.size();
        names.emplace_back(node->mName.C_Str());
        nameForNode[newNode] = nameIndex;
        std::cout << "node: " << node->mName.C_Str() << std::endl;
    }

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        int subNode = addNode(newNode, level + 1);

        const size_t nameIndex = names.size();
        names.push_back(std::string(node->mName.C_Str()) + "_Mesh_" + std::to_string(i));
        nameForNode[subNode] = nameIndex;

        const unsigned int meshIndex = node->mMeshes[i];
        meshMap[subNode] = meshIndex;
        materialForNode[subNode] = scene->mMeshes[meshIndex]->mMaterialIndex;

        std::cout << "material name:" << materialNames[scene->mMeshes[meshIndex]->mMaterialIndex] << std::endl;

        // values already given by the addNode() function
        // globalTransforms[subNode] = glm::mat4(1.0f);
        // localTransforms[subNode] = glm::mat4(1.0f);
    }

    // value already given by the addNode() function
    // globalTransforms[newNode] = glm::mat4(1.0f);
    localTransforms[newNode] = toMat4(node->mTransformation);
    printMat4(localTransforms[newNode]);

    for (unsigned int child = 0; child < node->mNumChildren; child++) {
        processNode(node->mChildren[child], scene, newNode, level + 1);
    }
}

Mesh SceneTree::processMesh(const aiMesh* mesh) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // process vertices
    vertices.resize(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex& vertex = vertices[i];
        // process vertex positions, normals and texture coordinates
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normals.x = mesh->mNormals[i].x;
        vertex.normals.y = mesh->mNormals[i].y;
        vertex.normals.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.uvs.x = mesh->mTextureCoords[0][i].x;
            vertex.uvs.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.uvs = glm::vec2(0.0f, 0.0f);
        }
    }

    // process indices
    const unsigned int faceIndicesCount = mesh->mFaces[0].mNumIndices;
    indices.resize(mesh->mNumFaces * faceIndicesCount);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices[i * faceIndicesCount + j] = face.mIndices[j];
        }
    }

    return {vertices, indices, mesh->mMaterialIndex};
}
