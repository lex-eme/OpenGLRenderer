#pragma once
#include <glad/glad.h>
#include <vector>

#include "../ShaderProgram.h"
#include "../Transform.h"
#include "glm/glm.hpp"


class Sphere {
public:
    Transform transform{};

private:
    int numVertices{};
    int numIndices{};
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    GLuint vbo[3]{};
    GLuint textureID{};

    void init(int);
    static float toRadians(float degrees);

public:
    Sphere();
    explicit Sphere(int precision);

    void create(GLuint texID);
    void draw(ShaderProgram shader, const glm::mat4& parentTransform) const;
};
