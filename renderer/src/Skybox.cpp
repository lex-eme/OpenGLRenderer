#include "pch.h"

#include "Skybox.h"

#include "Image.h"

void Skybox::create(const std::vector<std::string>& paths) {
    assert(paths.size() == 6);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    for (int i = 0; i < paths.size(); i += 1) {
        if (Image image; image.loadFromFile(paths[i])) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.width, image.height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, image.data);
        }
    }

    shader = ShaderProgram::createShaderProgram("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
    glBindVertexArray(0);
}

void Skybox::draw() const {
    glBindVertexArray(vao);

    shader.setActive();
    shader.setIntUniform("skybox", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

    glBindVertexArray(0);
}
