#pragma once
#include "Scene.h"


class LightScene final : public Scene {
    glm::vec3 lightPos{0.0f, 0.0f, 3.0f};
    glm::vec3 lightAmbient{0.2f, 0.2f, 0.2f};
    glm::vec3 lightDiffuse{0.5f, 0.5f, 0.5f};
    glm::vec3 lightSpecular{1.0f, 1.0f, 1.0f};


    float shininess = 32.0f;

public:
    explicit LightScene(sf::Window& inWindow);

    void load(float width, float height) override;
    void draw() const override;
    void drawDebug() override;
};
