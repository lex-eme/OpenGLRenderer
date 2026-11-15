#pragma once

#include <Bottleneck.h>
#include <EntryPoint.h>

#include "TestLayer.h"


class Sandbox final : public Bottleneck::Application {
public:
    Sandbox() {
        pushLayer(new TestLayer());
    }

    ~Sandbox() {

    }


};

inline Bottleneck::Application* Bottleneck::CreateApplication() {
    return new Sandbox();
}
