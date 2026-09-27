#include "Mesh.h"

#include <fstream>
#include <map>

void Mesh::loadFromFile(const std::string& path) {
    std::ifstream fin(path);
    std::string type;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::vector<Vertex> vertices;
    std::vector<glm::vec3> facePositions;
    std::vector<glm::vec2> faceTexCoords;
    std::vector<int> indices;
    std::map<std::tuple<int, int>, int> indexMap;

    int index = 0;

    while (fin >> type) {
        if (type == "v") {
            // v 1.000000 1.000000 -1.000000
            float x, y, z;
            fin >> x >> y >> z;
            positions.emplace_back(x, y, z);
        } else if (type == "vn") {
            // vn -0.0000 1.0000 -0.0000
            float x, y, z;
            fin >> x >> y >> z;
            normals.emplace_back(x, y, z);
        } else if (type == "vt") {
            // vt 0.875000 0.500000
            float u, v;
            fin >> u >> v;
            uvs.emplace_back(u, v);
        } else if (type == "f") {
            // f 5/1/1 3/2/1 1/3/1
            int v1, t1, n1;
            char skip;

            for (size_t i = 0; i < 3; i++) {
                fin >> v1 >> skip >> t1 >> skip >> n1;
                std::tuple tuple{v1, t1};
                if (auto search = indexMap.find(tuple); search != indexMap.end()) {
                    indices.push_back(search->second);
                } else {
                    vertices.emplace_back(positions[v1 - 1], uvs[t1 - 1]);
                    indices.emplace_back(index);
                    indexMap.insert({tuple, index});
                    index += 1;
                }
            }
        }
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    createVertexBuffer(vertices);
    createIndexBuffer(indices);
    indicesCount = indices.size();
    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(vao);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CCW);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::createVertexBuffer(const std::vector<Vertex>& vertices) {
    glGenBuffers(1, &verticesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(5* sizeof(float)));
    //glEnableVertexAttribArray(2);
}

void Mesh::createIndexBuffer(const std::vector<int>& indices) {
    glGenBuffers(1, &indicesVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
}
