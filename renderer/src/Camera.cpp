#include "Camera.h"

#include "imgui.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

void Camera::init(const float aspectRatio) {
    setAspectRatio(aspectRatio);
    viewMatrix = glm::lookAt(position, position + forward, worldUp);
}

void Camera::update() {
    viewMatrix = glm::lookAt(position, position + forward, up);
}

void Camera::setAspectRatio(const float ratio) {
    aspectRatio = ratio;
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
}

void Camera::move(const glm::vec3 movement) {
    position += movement;
}

void Camera::rotate(const glm::vec2 delta) {
    yaw += delta.x * sensitivity;
    pitch += delta.y * sensitivity;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(direction);
    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
}

const glm::vec3& Camera::getForward() const {
    return forward;
}

const glm::vec3& Camera::getRight() const {
    return right;
}

const glm::vec3& Camera::getUp() const {
    return up;
}

void Camera::drawDebug() {
    ImGui::SeparatorText("Camera");
    if (ImGui::DragFloat("FOV", &fov, 1.0f, 30.0f, 120.0f)) {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
    }
    ImGui::DragFloat("Sensitivity", &sensitivity, 0.01f, 0.01f, 0.5f);
}
