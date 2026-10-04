#pragma once
#include "Scene.h"


class TestScene final : public Scene {
public:
    explicit TestScene(sf::Window& inWindow);
    void load(float width, float height) override;
};
