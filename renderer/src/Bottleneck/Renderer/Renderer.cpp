#include "Renderer.h"

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Bottleneck/Debug/Instrumentor.h"

namespace Bottleneck {
    void Renderer::Init() {
        BN_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown() {
        BN_PROFILE_FUNCTION();

        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResize(const uint32_t width, const uint32_t height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(const OrthographicCamera& camera) {
        BN_PROFILE_FUNCTION();

        s_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::EndScene() {
        BN_PROFILE_FUNCTION();
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
        BN_PROFILE_FUNCTION();

        shader->bind();
        shader->setMat4("u_ViewProjection", s_sceneData->viewProjectionMatrix);
        shader->setMat4("u_Transform", transform);
        vertexArray->bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
