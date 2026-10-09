#pragma once
#include "Scene.h"


class LightScene final : public Scene {
    glm::vec3 objectColor{1.0f, 0.5f, 0.31f};
    glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
    float ambientStrength = 0.1f;

    glm::vec3 lightPos{1.0f, 1.0f, 2.0f};

public:
    explicit LightScene(sf::Window& inWindow);

    void load(float width, float height) override;
    void draw() const override;
    void drawDebug() override;
};
