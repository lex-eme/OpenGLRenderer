#include "Transform.h"

#include "glm/ext/matrix_transform.hpp"

void Transform::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
    computeMatrix();
}

void Transform::setRotation(const glm::vec3& newRotation) {
    rotation = newRotation;
    computeMatrix();
}

void Transform::setScale(const glm::vec3& newScale) {
    scale = newScale;
    computeMatrix();
}

const glm::mat4& Transform::getMatrix() const {
    return transformMatrix;
}

void Transform::computeMatrix() {
    transformMatrix = glm::translate(glm::mat4(1.0f), position);
    transformMatrix *= glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0, 0.0, 0.0));
    transformMatrix *= glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0, 1.0, 0.0));
    transformMatrix *= glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0, 0.0, 1.0));
    transformMatrix *= glm::scale(glm::mat4(1.0f), scale);
}
