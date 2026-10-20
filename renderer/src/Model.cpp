#include "Model.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string& filePath) {
    loadModel(filePath);
}

void Model::draw(const ShaderProgram shader) {
    shader.setMat4Uniform("model_matrix", transform.getMatrix());
    for (Mesh& mesh: meshes) {
        mesh.draw(shader);
    }
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

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

    // process material
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(const aiMaterial* mat, const aiTextureType type,
                                                 const std::string& typeName) {
    std::vector<Texture> textures;
    textures.resize(mat->GetTextureCount(type));
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        for (Texture& loadedTexture: loadedTextures) {
            if (std::strcmp(loadedTexture.path.data(), str.C_Str()) == 0) {
                textures[i] = loadedTexture;
                skip = true;
                break;
            }
        }

        if (!skip) {
            std::string filename(str.C_Str());
            filename = directory + "/" + filename;
            textures[i].loadFromFile(filename);
            textures[i].type = typeName;
            textures[i].path = str.C_Str();
            loadedTextures.push_back(textures[i]);
        }
    }
    return textures;
}
