#pragma once

#include "Bottleneck.h"


class Sandbox2D final : public Bottleneck::Layer {
    Bottleneck::OrthographicCameraController m_cameraController;

    // Temp
    Bottleneck::Ref<Bottleneck::VertexArray> m_vertexArray;
    Bottleneck::Ref<Bottleneck::Texture2D> m_texture;

    glm::vec4 m_color{0.8f, 0.2f, 0.3f, 1.0f};

public:
    Sandbox2D();

    void onAttach() override;
    void onDetach() override;
    void onUpdate(Bottleneck::Timestep timestep) override;
    void onImGuiRender() override;
    void onEvent(Event& event) override;
};
