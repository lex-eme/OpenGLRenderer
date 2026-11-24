#pragma once
#include "Renderer/RendererAPI.h"

namespace Bottleneck {
    class OpenGLRendererAPI final : public RendererAPI {
    public:
        OpenGLRendererAPI() = default;
        ~OpenGLRendererAPI() override = default;

        void init() override;
        void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void setClearColor(const glm::vec4& color) override;
        void clear() override;
        void drawIndexed(const Ref<VertexArray>& vertexArray) override;
    };
}
