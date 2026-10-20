#include "Mesh.h"

#include <fstream>
#include <map>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
           const std::vector<Texture>& inTextures) {
    setupMesh(vertices, indices);
    textures = inTextures;
}

void Mesh::unload() {
    // Unbind vao before the others to avoid modifying unwanted vao
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteVertexArrays(1, &vao);

    vbo = 0;
    ebo = 0;
    vao = 0;
}

void Mesh::draw(const ShaderProgram shader) const {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    glBindVertexArray(vao);

    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr);
            diffuseNr += 1;
        } else if (name == "texture_specular") {
            number = std::to_string(specularNr);
            specularNr += 1;
        }

        std::string uniformName = "material." + name;
        uniformName += number;
        shader.setIntUniform(uniformName, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CCW);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    indicesCount = indices.size();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normals)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uvs)));

    glBindVertexArray(0);
}
