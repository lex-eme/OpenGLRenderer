#pragma once
#include "../shader/ShaderProgram.h"


class Material {
public:
    std::string name{};
    ShaderProgram shader{};

    Material() = default;
    virtual ~Material() = default;

    virtual void bind() const = 0;
    virtual void drawDebug() = 0;
};
