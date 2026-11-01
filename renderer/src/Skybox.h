#pragma once
#include "shader/ShaderProgram.h"


class Skybox {
public:
    ShaderProgram shader;

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int texId;


public:
    Skybox() = default;

    void create(const std::vector<std::string>& paths);

    void draw() const;
};
