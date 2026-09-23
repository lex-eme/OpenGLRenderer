#pragma once
#include "glm/glm.hpp"
#include "glm/vec3.hpp"


struct Transform {
private:
    glm::vec3 position{};
    glm::vec3 rotation{};
    glm::vec3 scale{1.0f};
    glm::mat4 transformMatrix{};

public:
    void setPosition(const glm::vec3& newPosition);
    void setRotation(const glm::vec3& newRotation);
    void setScale(const glm::vec3& newScale);

    [[nodiscard]] const glm::mat4& getMatrix() const;

private:
    void computeMatrix();
};
