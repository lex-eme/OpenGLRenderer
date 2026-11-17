#include "pch.h"

#include "Buffer.h"

#include "platform/openGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Bottleneck {
    VertexBuffer* VertexBuffer::Create(const float* vertices, const uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                assert(false);
            case RendererAPI::API::OpenGL:
                return new OpenGLVertexBuffer(vertices, size);
        }

        assert(false);
    }

    IndexBuffer* IndexBuffer::Create(const uint32_t* indices, const uint32_t count) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                assert(false);
            case RendererAPI::API::OpenGL:
                return new OpenGLIndexBuffer(indices, count);
        }

        assert(false);
    }
}
