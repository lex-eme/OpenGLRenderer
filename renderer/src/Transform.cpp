#include "Transform.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"
#include "imgui/imgui.h"

Transform::Transform() {
    computeMatrix();
}

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

void Transform::drawDebug() {
    if (ImGui::DragFloat3("Location", glm::value_ptr(position), 0.1f, 0.0f, 0.0f, "%.3f", ImGuiSliderFlags_ColorMarkers)) {
        computeMatrix();
    }
    if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f, 0.0f, 0.0f, "%.3f", ImGuiSliderFlags_ColorMarkers)) {
        computeMatrix();
    }
    if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f, 0.0f, 0.0f, "%.3f", ImGuiSliderFlags_ColorMarkers)) {
        computeMatrix();
    }
}

const glm::mat4& Transform::getMatrix() const {
    return transformMatrix;
}

void Transform::computeMatrix() {
    transformMatrix = glm::translate(glm::mat4(1.0f), position);
    transformMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
    transformMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
    transformMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
    transformMatrix *= glm::scale(glm::mat4(1.0f), scale);
}
