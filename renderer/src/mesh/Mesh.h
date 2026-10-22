#pragma once
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normals;
    glm::vec2 uvs;
};

class Mesh {
public:
    unsigned int vao{};
    unsigned int vbo{};
    unsigned int ebo{};
    int indicesCount{};
    unsigned int materialIndex;

public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int index);

    void draw() const;
    void unload();

private:
    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};
