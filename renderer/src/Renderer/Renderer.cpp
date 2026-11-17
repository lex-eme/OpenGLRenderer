#include "Renderer.h"

#include "RenderCommand.h"

namespace Bottleneck {
    void Renderer::BeginScene() {
    }

    void Renderer::EndScene() {
    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
        vertexArray->bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
