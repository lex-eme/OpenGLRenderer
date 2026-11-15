#pragma once

#include <Bottleneck.h>


class TestLayer final : public Bottleneck::Layer {
public:
    TestLayer();
    ~TestLayer() override = default;
    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    void onEvent(Event& event) override;
    void onImGuiRender() override;
};
