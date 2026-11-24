#pragma once

#include "RendererAPI.h"
#include "platform/openGL/OpenGLRendererAPI.h"

namespace Bottleneck {
    class RenderCommand {
        inline static RendererAPI* s_RendererAPI = new OpenGLRendererAPI();

    public:
        static void Init() { s_RendererAPI->init(); }

        static void SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) {
            s_RendererAPI->setViewport(x, y, width, height);
        }

        static void SetClearColor(const glm::vec4& color) { s_RendererAPI->setClearColor(color); }
        static void Clear() { s_RendererAPI->clear(); }

        static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
            s_RendererAPI->drawIndexed(vertexArray);
        }
    };
}
