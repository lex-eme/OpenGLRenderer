#include "Model.h"

#include <iostream>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ImGuiUtils.h"
#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.inl"
#include "scene/Scene.h"

Model::Model(const std::string& filePath, const bool flipUvs) {
    loadModel(filePath, flipUvs);
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
        ImGui::SeparatorText("Transform");
        transform.drawDebug();

        ImGui::SeparatorText("Materials");
        const float materialTableHeight = ImGuiUtils::TableHeight(static_cast<int>(materials.size()), 4);
        if (ImGui::BeginChild("MaterialTableFrame", ImVec2(0, materialTableHeight), ImGuiChildFlags_Borders)) {
            if (ImGui::BeginTable("MaterialTable", 3,
                                  ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY |
                                  ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Index");
                ImGui::TableSetupColumn("Slot");
                ImGui::TableSetupColumn("Material");
                ImGui::TableHeadersRow();

                for (int i = 0; i < materials.size(); i++) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", i);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", materialNames[i].c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::PushID(i);
                    if (ImGui::BeginCombo("", materials[i]->name.c_str(),
                                          ImGuiComboFlags_WidthFitPreview | ImGuiComboFlags_NoArrowButton)) {
                        for (auto& [name, material]: Scene::assetManager.getMaterialMap()) {
                            const bool isSelected = materials[i] == material;
                            if (ImGui::Selectable(name.c_str(), isSelected)) {
                                materials[i] = material;
                            }
                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (isSelected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        ImGui::SeparatorText("Mesh list");
        const float meshesTableHeight = ImGuiUtils::TableHeight(static_cast<int>(meshes.size()), 6);
        if (ImGui::BeginChild("MeshTableFrame", ImVec2(0, meshesTableHeight), ImGuiChildFlags_Borders)) {
            if (ImGui::BeginTable("MeshTable", 4,
                                  ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Index");
                ImGui::TableSetupColumn("Material");
                ImGui::TableSetupColumn("Vertices");
                ImGui::TableSetupColumn("Indices");
                ImGui::TableHeadersRow();

                for (int i = 0; i < meshes.size(); i++) {
                    const Mesh& mesh = meshes[i];
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", i);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", mesh.materialIndex);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%d", mesh.vertexCount);
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%d", mesh.indicesCount);
                }
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();
    }
    ImGui::PopID();
}

void Model::loadModel(const std::string& path, const bool flipUvs) {
    unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
    flags |= flipUvs ? aiProcess_FlipUVs : 0;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    for (int i = 0; i < scene->mNumMaterials; i += 1) {
        const aiMaterial* material = scene->mMaterials[i];
        materials.emplace_back();
        materialNames.emplace_back(material->GetName().C_Str());
    }

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
