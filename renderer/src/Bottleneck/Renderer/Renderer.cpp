#include "Renderer.h"

#include "RenderCommand.h"
#include "Shader.h"

namespace Bottleneck {
    void Renderer::Init() {
        RenderCommand::Init();
    }

    void Renderer::OnWindowResize(const uint32_t width, const uint32_t height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(const OrthographicCamera& camera) {
        s_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::EndScene() {
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
        shader->bind();
        shader->uploadUniformMat4("u_ViewProjection", s_sceneData->viewProjectionMatrix);
        shader->uploadUniformMat4("u_Transform", transform);
        vertexArray->bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
