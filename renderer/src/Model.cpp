#include "Model.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.inl"

Model::Model(const std::string& filePath) {
    loadModel(filePath);
}

void Model::draw() {
    for (const Material* material: materials) {
        material->shader.setMat4Uniform("model", transform.getMatrix());
    }

    for (Mesh& mesh: meshes) {
        materials[mesh.materialIndex]->bind();
        mesh.draw();
    }
}

void Model::drawDebug(const int id) {
    ImGui::PushID(id);
    if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_None)) {
        transform.drawDebug();
    }
    ImGui::PopID();
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    std::cout << "Material count: " << scene->mNumMaterials << std::endl;

    for (int i = 0; i < scene->mNumMaterials; i += 1) {
        const aiMaterial* material = scene->mMaterials[i];
        std::cout << "Material name: " << material->GetName().C_Str() << std::endl;
        materials.emplace_back();
    }

    std::cout << "Model mesh count: " << scene->mNumMeshes << std::endl;

    processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(const aiMesh* mesh) {
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

    return Mesh(vertices, indices, mesh->mMaterialIndex);
}
