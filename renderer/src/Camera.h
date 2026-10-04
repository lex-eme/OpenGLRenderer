#pragma once

#include <glm/glm.hpp>

constexpr glm::vec3 worldUp{0.0f, 1.0f, 0.0f};


class Camera {
public:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    float sensitivity = 0.2f;

private:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 60.0f;
    float aspectRatio;

public:
    Camera() = default;

    void init(float aspectRatio);
    void update();
    void setAspectRatio(float ratio);
    void move(glm::vec3 movement);
    void rotate(glm::vec2 delta);

    const glm::vec3& getForward() const;
    const glm::vec3& getRight() const;
    const glm::vec3& getUp() const;

    void drawDebug();
};
