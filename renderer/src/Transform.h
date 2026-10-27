#pragma once
#include "glm/glm.hpp"


struct Transform {
private:
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
    glm::mat4 transformMatrix{};

public:
    Transform();

    void setPosition(const glm::vec3& newPosition);
    void setRotation(const glm::vec3& newRotation);
    void setScale(const glm::vec3& newScale);

    void drawDebug();

    [[nodiscard]] const glm::mat4& getMatrix() const;

private:
    void computeMatrix();
};
