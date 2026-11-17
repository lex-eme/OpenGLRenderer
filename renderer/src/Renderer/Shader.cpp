#include "pch.h"

#include "Shader.h"

#include "Renderer.h"
#include "platform/openGL/OpenGLShader.h"

namespace Bottleneck {
    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::None:
                assert(false);
            case RenderAPI::OpenGL:
                return new OpenGLShader(vertexSrc, fragmentSrc);
        }

        assert(false);
    }
}
