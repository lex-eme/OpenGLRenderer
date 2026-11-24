#pragma once

#include <memory>
#include <vector>

#include "Bottleneck/Renderer/VertexArray.h"

namespace Bottleneck {
    class OpenGLVertexArray final : public VertexArray {
        uint32_t m_rendererID{};
        std::vector<Ref<VertexBuffer>> m_vertexBuffers;
        Ref<IndexBuffer> m_indexBuffer;

    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        [[nodiscard]] const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override {
            return m_vertexBuffers;
        }

        [[nodiscard]] const Ref<IndexBuffer>& getIndexBuffer() const override {
            return m_indexBuffer;
        }
    };
}
