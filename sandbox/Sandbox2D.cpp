#include "Sandbox2D.h"

#include <glm/gtc/type_ptr.inl>
#include <glm/ext/matrix_transform.hpp>
#include "../dependencies/imgui/imgui.h"
#include "Bottleneck/Debug/Instrumentor.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox 2D"), m_cameraController(1.0f) {
}

void Sandbox2D::onAttach() {
    BN_PROFILE_FUNCTION();

    m_texture = Bottleneck::Texture2D::Create("assets/textures/vince.png");
}

void Sandbox2D::onDetach() {
    BN_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(const Bottleneck::Timestep timestep) {
    BN_PROFILE_FUNCTION();
    // Update
    m_cameraController.onUpdate(timestep);

    // Render
    {
        BN_PROFILE_SCOPE("Renderer Prep");
        Bottleneck::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Bottleneck::RenderCommand::Clear();
    } {
        BN_PROFILE_SCOPE("Renderer Draw");
        Bottleneck::Renderer2D::BeginScene(m_cameraController.getCamera());
        Bottleneck::Renderer2D::DrawRotatedQuad({-1.0f, 0.0f, -0.1f}, {1.0f, 1.0f}, glm::radians(15.0f), m_color);
        Bottleneck::Renderer2D::DrawRotatedQuad({0.5f, -0.5f}, {0.5f, 0.75f}, glm::radians(-15.0f), m_color);
        Bottleneck::Renderer2D::DrawRotatedQuad({-0.75f, 0.0f}, {0.6f, 0.6f}, glm::radians(45.0f), m_texture, 2.0f,
                                                {1.0f, 0.2f, 0.2f, 1.0f});
        Bottleneck::Renderer2D::EndScene();
    }
}

void Sandbox2D::onImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Color", glm::value_ptr(m_color));
    ImGui::End();
}

void Sandbox2D::onEvent(Event& event) {
    m_cameraController.onEvent(event);
}
