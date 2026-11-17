#pragma once

#include <memory>
#include <vector>

#include "Renderer/VertexArray.h"

namespace Bottleneck {
    class OpenGLVertexArray final : public VertexArray {
        uint32_t m_rendererID{};
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;

    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

        [[nodiscard]] const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override {
            return m_vertexBuffers;
        }

        [[nodiscard]] const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override {
            return m_indexBuffer;
        }
    };
}
