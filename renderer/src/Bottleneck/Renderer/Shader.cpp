#include "pch.h"

#include "Shader.h"

#include "Renderer.h"
#include "platform/openGL/OpenGLShader.h"

namespace Bottleneck {
    Ref<Shader> Shader::Create(const std::string& filepath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                assert(false);
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(filepath);
        }

        assert(false);
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                assert(false);
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }

        assert(false);
    }

    void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader) {
        if (m_shaders.contains(name)) {
            BN_CORE_WARN("Shader library already contains a shader named '{0}'", name);
        }
        m_shaders[name] = shader;
    }

    void ShaderLibrary::add(const Ref<Shader>& shader) {
        auto& name = shader->getName();
        add(name, shader);
    }

    Ref<Shader> ShaderLibrary::load(const std::string& filepath) {
        Ref<Shader> shader = Shader::Create(filepath);
        add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath) {
        Ref<Shader> shader = Shader::Create(filepath);
        add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::get(const std::string& name) {
        if (!m_shaders.contains(name)) {
            BN_CORE_WARN("Shader library does not contain a shader named '{0}'", name);
        }
        return m_shaders[name];
    }
}
