#pragma once
#include "Renderer/RendererAPI.h"

namespace Bottleneck {
    class OpenGLRendererAPI final : public RendererAPI {
    public:
        OpenGLRendererAPI() = default;
        ~OpenGLRendererAPI() override = default;

        void setClearColor(const glm::vec4& color) override;
        void clear() override;
        void drawIndexed(const Ref<VertexArray>& vertexArray) override;
    };
}
