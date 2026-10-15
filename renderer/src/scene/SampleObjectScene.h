#pragma once
#include "Scene.h"


class SampleObjectScene final : public Scene {
    float angle = 0.0f;

public:
    SampleObjectScene() = default;
    void load(GLFWwindow* inWindow, float width, float height) override;

    void draw() const override;
    void drawDebug() override;
};
