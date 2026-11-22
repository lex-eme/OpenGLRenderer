#pragma once

#include <Bottleneck.h>
#include <core/Core.h>


class TestLayer final : public Bottleneck::Layer {
    Bottleneck::ShaderLibrary m_shaderLibrary;
    Bottleneck::Ref<Bottleneck::Shader> m_shader;
    Bottleneck::Ref<Bottleneck::VertexArray> m_vertexArray;
    Bottleneck::Ref<Bottleneck::Texture2D> m_texture;

    Bottleneck::OrthographicCamera m_camera;
    glm::vec3 m_cameraPosition{0.0f};
    float m_cameraSpeed{1.0f};

    float m_cameraRotation{0.0f};
    float m_cameraRotationSpeed{30.0f};

    glm::vec3 m_trianglePosition{0.0f};
    glm::vec3 m_triangleColor{0.5f};

public:
    TestLayer();
    ~TestLayer() override = default;
    void onAttach() override;
    void onDetach() override;
    void onUpdate(Bottleneck::TimeStep timeStep) override;
    void onEvent(Event& event) override;
    void onImGuiRender() override;

private:
    bool onKeyPressedEvent(const KeyPressedEvent& event);
};
