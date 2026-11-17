#include "pch.h"

#include "VertexArray.h"

#include "Renderer.h"
#include "platform/openGL/OpenGLVertexArray.h"

namespace Bottleneck {
    VertexArray* VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RenderAPI::None:
                assert(false);
            case RenderAPI::OpenGL:
                return new OpenGLVertexArray();
        }

        assert(false);
    }
}
