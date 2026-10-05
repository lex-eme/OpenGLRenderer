#pragma once
#include "Scene.h"


class SampleObjectScene final : public Scene {
    float angle = 0.0f;

public:
    explicit SampleObjectScene(sf::Window& inWindow);
    void load(float width, float height) override;

    void drawDebug() override;
};
