#pragma once
#include "Renderer/Shader.h"

namespace Bottleneck {
    class OpenGLShader final : public Shader {
        uint32_t m_rendererID;

    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader() override;

        void bind() const override;
        void unbind() const override;
    };
}
