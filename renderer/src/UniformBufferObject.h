#pragma once

#include <glm/glm.hpp>

class UniformBufferObject {
public:
    unsigned int bindingPoint = 0;

private:
    unsigned int ubo{};

public:
    UniformBufferObject() = default;

    void create();
    void setProjection(const glm::mat4& matrix) const;
    void setView(const glm::mat4& matrix) const;
};
