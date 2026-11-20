#pragma once
#include "Renderer/Texture.h"

namespace Bottleneck {
    class OpenGLTexture2D final : public Texture2D {
        std::string m_path;
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_RendererID;

    public:
        explicit OpenGLTexture2D(const std::string& path);
        ~OpenGLTexture2D() override;

        [[nodiscard]] uint32_t getWidth() const override { return m_width; }
        [[nodiscard]] uint32_t getHeight() const override { return m_height; }

        void bind(uint32_t slot) const override;
    };
}
