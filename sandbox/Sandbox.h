#pragma once

#include <Bottleneck.h>
#include <Bottleneck/Core/EntryPoint.h>

#include "TestLayer.h"


class Sandbox final : public Bottleneck::Application {
public:
    Sandbox() {
        pushLayer(new TestLayer());
    }

    ~Sandbox() override = default;
};

inline Bottleneck::Application* Bottleneck::CreateApplication() {
    return new Sandbox();
}
