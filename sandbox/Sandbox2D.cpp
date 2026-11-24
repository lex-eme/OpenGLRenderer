#include "Sandbox2D.h"

#include <glm/gtc/type_ptr.inl>
#include <glm/ext/matrix_transform.hpp>
#include "../dependencies/imgui/imgui.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox 2D"), m_cameraController(1.0f) {
}

void Sandbox2D::onAttach() {

}

void Sandbox2D::onDetach() {
}

void Sandbox2D::onUpdate(const Bottleneck::Timestep timestep) {
    // Update
    m_cameraController.onUpdate(timestep);

    // Render
    Bottleneck::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Bottleneck::RenderCommand::Clear();

    Bottleneck::Renderer2D::BeginScene(m_cameraController.getCamera());
    Bottleneck::Renderer2D::DrawQuad({-1.0f, 0.0f}, {1.0f, 1.0f}, m_color);
    Bottleneck::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, m_color);
    Bottleneck::Renderer2D::EndScene();
}

void Sandbox2D::onImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Color", glm::value_ptr(m_color));
    ImGui::End();
}

void Sandbox2D::onEvent(Event& event) {
    m_cameraController.onEvent(event);
}
