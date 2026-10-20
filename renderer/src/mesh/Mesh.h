#pragma once
#include <glm/glm.hpp>

#include "../Texture.h"
#include "../shader/ShaderProgram.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normals;
    glm::vec2 uvs;
};

class Mesh {
    unsigned int vao{};
    unsigned int vbo{};
    unsigned int ebo{};
    int indicesCount{};

    std::vector<Texture> textures;

public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
         const std::vector<Texture>& inTextures);

    void draw(ShaderProgram shader) const;
    void unload();

private:
    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};
