#pragma once
#include "../Transform.h"

class Material;

class Grid {
    Transform transform{};
    unsigned int vao{};
    unsigned int vbo{};
    int verticeCount{};
    Material* material{};

public:
    Grid() = default;

    void create(Material* inMaterial);
    void draw() const;

private:
    static bool checkOpenGLError();
};
