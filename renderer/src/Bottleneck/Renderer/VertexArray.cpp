#include "pch.h"

#include "VertexArray.h"

#include "Renderer.h"
#include "platform/openGL/OpenGLVertexArray.h"

namespace Bottleneck {
    Ref<VertexArray> VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                assert(false);
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexArray>();
        }

        assert(false);
    }
}
