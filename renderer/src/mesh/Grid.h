#pragma once
#include "../Transform.h"
#include "../material/GridMaterial.h"
#include "../UniformBufferObject.h"

class Camera;

class Grid {
    Transform transform{};
    unsigned int vao{};
    unsigned int vbo{};
    int verticeCount{};
    GridMaterial material;

public:
    Grid() = default;

    void create(Camera* camera, UniformBufferObject ubo);
    void draw() const;

private:
    static bool checkOpenGLError();
};
