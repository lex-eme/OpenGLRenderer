#include "pch.h"

#include "Shader.h"

#include "Renderer.h"
#include "platform/openGL/OpenGLShader.h"

namespace Bottleneck {
    Shader* Shader::Create(const std::string& filepath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                assert(false);
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(filepath);
        }

        assert(false);
    }

    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                assert(false);
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertexSrc, fragmentSrc);
        }

        assert(false);
    }
}
