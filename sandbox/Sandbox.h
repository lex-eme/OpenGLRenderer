#pragma once

#include <Bottleneck.h>
#include <Bottleneck/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "TestLayer.h"


class Sandbox final : public Bottleneck::Application {
public:
    Sandbox() {
        //pushLayer(new TestLayer());
        pushLayer(new Sandbox2D);
    }

    ~Sandbox() override = default;
};

inline Bottleneck::Application* Bottleneck::CreateApplication() {
    return new Sandbox();
}
