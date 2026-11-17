#pragma once

#include "RendererAPI.h"
#include "platform/openGL/OpenGLRendererAPI.h"

namespace Bottleneck {
    class RenderCommand {
        inline static RendererAPI* s_RendererAPI = new OpenGLRendererAPI();

    public:
        static void SetClearColor(const glm::vec4& color) { s_RendererAPI->setClearColor(color); }
        static void Clear() { s_RendererAPI->clear(); }

        static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
            s_RendererAPI->drawIndexed(vertexArray);
        }
    };
}
