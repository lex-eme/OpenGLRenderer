#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>


struct Vertex {
    glm::vec3 position;
    glm::vec2 uvs;
    glm::vec3 normals;
};

class Mesh {
    GLuint vao{};
    GLuint vbo{};
    GLuint ebo{};
    GLsizei indicesCount{};

public:
    void loadFromFile(const std::string &path);
    void unload();

    void draw() const;

private:
    void createVertexBuffer(const std::vector<Vertex>& vertices);
    void createIndexBuffer(const std::vector<int>& indices);
};
