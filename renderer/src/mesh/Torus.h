#pragma once
#include <glad/glad.h>
#include "../ShaderProgram.h"
#include "../Transform.h"
#include "glm/glm.hpp"

class Torus {
public:
    Transform transform{};

private:
    int numVertices{};
    int numIndices{};
    int precision;
    float inner;
    float outer;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> sTangents;
    std::vector<glm::vec3> tTangents;

    GLuint vao;
    GLuint vbo[4]{};
    GLuint textureID{};

    void init();
    static float toRadians(float degrees);

public:
    Torus();
    Torus(float inner, float outer, int precision);

    void create(GLuint texID);
    void draw(ShaderProgram shader, const glm::mat4& parentTransform) const;
};