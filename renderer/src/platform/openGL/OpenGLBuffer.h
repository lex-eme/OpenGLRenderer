#pragma once
#include "Renderer/Buffer.h"

namespace Bottleneck {
    class OpenGLVertexBuffer final : public VertexBuffer {
        uint32_t m_rendererID{};
        BufferLayout m_layout;

    public:
        OpenGLVertexBuffer(const float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void bind() const override;
        void unbind() const override;

        [[nodiscard]] const BufferLayout& getLayout() const override {
            return m_layout;
        }
        void setLayout(const BufferLayout& layout) override;
    };

    class OpenGLIndexBuffer final : public IndexBuffer {
        uint32_t m_rendererID{};
        uint32_t m_count{};

    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void bind() const override;
        void unbind() const override;

        [[nodiscard]] uint32_t getCount() const override {
            return m_count;
        }
    };
}
