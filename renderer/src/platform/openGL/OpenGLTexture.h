#pragma once

#include <glad/glad.h>

#include "Bottleneck/Renderer/Texture.h"

namespace Bottleneck {
    class OpenGLTexture2D final : public Texture2D {
        std::string m_path;
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_RendererID;
        GLint m_internalFormat;
        GLenum m_dataFormat;

    public:
        explicit OpenGLTexture2D(uint32_t width, uint32_t height);
        explicit OpenGLTexture2D(const std::string& path);
        ~OpenGLTexture2D() override;

        [[nodiscard]] uint32_t getWidth() const override { return m_width; }
        [[nodiscard]] uint32_t getHeight() const override { return m_height; }

        void setData(void* data, uint32_t size) override;

        void bind(uint32_t slot) const override;
    };
}
