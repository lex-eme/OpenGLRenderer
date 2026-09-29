#include "Sphere.h"

Sphere::Sphere() {
    init(48);
}

Sphere::Sphere(const int precision) {
    init(precision);
}

void Sphere::create(const GLuint texID) {
    textureID = texID;

    std::vector<float> pvalues;
    std::vector<float> tvalues;
    std::vector<float> nvalues;

    pvalues.reserve(numIndices * 3);
    tvalues.reserve(numIndices * 2);
    nvalues.reserve(numIndices * 3);

    for (int i = 0; i < numIndices; i++) {
        pvalues.push_back(vertices[indices[i]].x);
        pvalues.push_back(vertices[indices[i]].y);
        pvalues.push_back(vertices[indices[i]].z);
        tvalues.push_back(texCoords[indices[i]].s);
        tvalues.push_back(texCoords[indices[i]].t);
        nvalues.push_back(normals[indices[i]].x);
        nvalues.push_back(normals[indices[i]].y);
        nvalues.push_back(normals[indices[i]].z);
    }

    glGenBuffers(3, vbo);

    // Positions buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    // Texture coordinates buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

    // Normals buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
}

void Sphere::draw(const ShaderProgram shader, const glm::mat4& parentTransform) const {
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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, numIndices);
}

float Sphere::toRadians(const float degrees) {
    return degrees * 2.0f * 3.14159f / 360.0f;
}

void Sphere::init(const int precision) {
    numVertices = (precision + 1) * (precision + 1);
    numIndices = precision * precision * 6;
    for (int i = 0; i < numVertices; i++) { vertices.emplace_back(); }
    for (int i = 0; i < numVertices; i++) { texCoords.emplace_back(); }
    for (int i = 0; i < numVertices; i++) { normals.emplace_back(); }
    for (int i = 0; i < numVertices; i++) { tangents.emplace_back(); }
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

    // calculate triangle vertices
    for (int i = 0; i <= precision; i++) {
        for (int j = 0; j <= precision; j++) {
            float y = cos(toRadians(180.0f - i * 180.0f / precision));
            float x = -cos(toRadians(j * 360.0f / precision)) * abs(cos(asin(y)));
            float z = sin(toRadians(j * 360.0f / (float) precision)) * abs(cos(asin(y)));
            vertices[i * (precision + 1) + j] = glm::vec3(x, y, z);
            texCoords[i * (precision + 1) + j] = glm::vec2(((float) j / precision), ((float) i / precision));
            normals[i * (precision + 1) + j] = glm::vec3(x, y, z);

            // calculate tangent vector
            if (((x == 0) && (y == 1) && (z == 0)) || ((x == 0) && (y == -1) && (z == 0))) {
                tangents[i * (precision + 1) + j] = glm::vec3(0.0f, 0.0f, -1.0f);
            } else {
                tangents[i * (precision + 1) + j] = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(x, y, z));
            }
        }
    }
    // calculate triangle indices
    for (int i = 0; i < precision; i++) {
        for (int j = 0; j < precision; j++) {
            indices[6 * (i * precision + j) + 0] = i * (precision + 1) + j;
            indices[6 * (i * precision + j) + 1] = i * (precision + 1) + j + 1;
            indices[6 * (i * precision + j) + 2] = (i + 1) * (precision + 1) + j;
            indices[6 * (i * precision + j) + 3] = i * (precision + 1) + j + 1;
            indices[6 * (i * precision + j) + 4] = (i + 1) * (precision + 1) + j + 1;
            indices[6 * (i * precision + j) + 5] = (i + 1) * (precision + 1) + j;
        }
    }
}
