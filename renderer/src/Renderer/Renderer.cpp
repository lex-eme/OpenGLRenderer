#include "Renderer.h"

#include "RenderCommand.h"
#include "Shader.h"

namespace Bottleneck {
    void Renderer::BeginScene(const OrthographicCamera& camera) {
        s_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::EndScene() {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {
        shader->bind();
        shader->uploadUniformMat4("u_ViewProjection", s_sceneData->viewProjectionMatrix);
        vertexArray->bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
