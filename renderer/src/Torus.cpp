#include "Torus.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Torus::Torus() {
    precision = 48;
    inner = 0.5f;
    outer = 0.2f;
    init();
}

Torus::Torus(const float in, const float out, const int precIn) {
    precision = precIn;
    inner = in;
    outer = out;
    init();
}

void Torus::create(const GLuint texID) {
    textureID = texID;

    std::vector<float> pvalues;
    std::vector<float> tvalues;
    std::vector<float> nvalues;

    pvalues.reserve(numVertices * 3);
    tvalues.reserve(numVertices * 2);
    nvalues.reserve(numVertices * 3);

    for (int i = 0; i < numVertices; i++) {
        pvalues.push_back(vertices[i].x);
        pvalues.push_back(vertices[i].y);
        pvalues.push_back(vertices[i].z);
        tvalues.push_back(texCoords[i].s);
        tvalues.push_back(texCoords[i].t);
        nvalues.push_back(normals[i].x);
        nvalues.push_back(normals[i].y);
        nvalues.push_back(normals[i].z);
    }

    glGenBuffers(4, vbo);

    // Positions buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

    // Texture coordinates buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

    // Normals buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

    // Indices buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);
}

void Torus::draw(const ShaderProgram shader, const glm::mat4& parentTransform) const {
    const glm::mat4 matrix = parentTransform * transform.getMatrix();
    shader.setMat4Uniform("mv_matrix", matrix);
    shader.setIntUniform("samp", 0);

    // Positions buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Texture coordinates buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Indices buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

float Torus::toRadians(const float degrees) {
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}

void Torus::init() {
    numVertices = (precision + 1) * (precision + 1);
    numIndices = precision * precision * 6;
    for (int i = 0; i < numVertices; i++) { vertices.emplace_back(); }
    for (int i = 0; i < numVertices; i++) { texCoords.emplace_back(); }
    for (int i = 0; i < numVertices; i++) { normals.emplace_back(); }
    for (int i = 0; i < numVertices; i++) { sTangents.emplace_back(); }
    for (int i = 0; i < numVertices; i++) { tTangents.emplace_back(); }
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

    // calculate first ring
    for (int i = 0; i < precision + 1; i++) {
        const float amt = toRadians(i * 360.0f / precision);

        glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 initPos(rMat * glm::vec4(0.0f, outer, 0.0f, 1.0f));

        vertices[i] = glm::vec3(initPos + glm::vec3(inner, 0.0f, 0.0f));
        texCoords[i] = glm::vec2(0.0f, static_cast<float>(i) / static_cast<float>(precision));

        rMat = glm::rotate(glm::mat4(1.0f), amt + (3.14159f / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        tTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));

        sTangents[i] = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
        normals[i] = glm::cross(tTangents[i], sTangents[i]);
    }

    // rotate the first ring about Y to get the other rings
    for (int ring = 1; ring < precision + 1; ring++) {
        for (int i = 0; i < precision + 1; i++) {
            const float amt = toRadians(static_cast<float>(ring) * 360.0f / precision);

            glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            vertices[ring * (precision + 1) + i] = glm::vec3(rMat * glm::vec4(vertices[i], 1.0f));

            texCoords[ring * (precision + 1) + i] = glm::vec2(
                static_cast<float>(ring) * 2.0f / static_cast<float>(precision), texCoords[i].t);

            rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            sTangents[ring * (precision + 1) + i] = glm::vec3(rMat * glm::vec4(sTangents[i], 1.0f));

            rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            tTangents[ring * (precision + 1) + i] = glm::vec3(rMat * glm::vec4(tTangents[i], 1.0f));

            rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
            normals[ring * (precision + 1) + i] = glm::vec3(rMat * glm::vec4(normals[i], 1.0f));
        }
    }

    // calculate triangle indices
    for (int ring = 0; ring < precision; ring++) {
        for (int i = 0; i < precision; i++) {
            indices[((ring * precision + i) * 2) * 3 + 0] = ring * (precision + 1) + i;
            indices[((ring * precision + i) * 2) * 3 + 1] = (ring + 1) * (precision + 1) + i;
            indices[((ring * precision + i) * 2) * 3 + 2] = ring * (precision + 1) + i + 1;
            indices[((ring * precision + i) * 2 + 1) * 3 + 0] = ring * (precision + 1) + i + 1;
            indices[((ring * precision + i) * 2 + 1) * 3 + 1] = (ring + 1) * (precision + 1) + i;
            indices[((ring * precision + i) * 2 + 1) * 3 + 2] = (ring + 1) * (precision + 1) + i + 1;
        }
    }
}
