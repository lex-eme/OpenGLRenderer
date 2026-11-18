#pragma once

#include <Bottleneck.h>


class TestLayer final : public Bottleneck::Layer {
    std::shared_ptr<Bottleneck::Shader> m_shader;
    std::shared_ptr<Bottleneck::VertexArray> m_vertexArray;

    Bottleneck::OrthographicCamera m_camera;
    glm::vec3 m_cameraPosition{0.0f};
    float m_cameraSpeed{0.001f};

    float m_cameraRotation{0.0f};
    float m_cameraRotationSpeed{0.1f};

public:
    TestLayer();
    ~TestLayer() override = default;
    void onAttach() override;
    void onDetach() override;
    void onUpdate() override;
    void onEvent(Event& event) override;
    void onImGuiRender() override;

private:
    bool onKeyPressedEvent(const KeyPressedEvent& event);
};
