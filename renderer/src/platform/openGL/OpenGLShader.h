#pragma once

#include <unordered_map>

#include "Renderer/Shader.h"

namespace Bottleneck {
    class OpenGLShader final : public Shader {
        uint32_t m_rendererID;

    public:
        explicit OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader() override;

        void bind() const override;
        void unbind() const override;

        void uploadUniformInt(const std::string& name, int value) override;

        void uploadUniformFloat(const std::string& name, float value) override;
        void uploadUniformFloat2(const std::string& name, const glm::vec2& value) override;
        void uploadUniformFloat3(const std::string& name, const glm::vec3& value) override;
        void uploadUniformFloat4(const std::string& name, const glm::vec4& value) override;

        void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) override;
        void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;

    private:
        static std::string readFile(const std::string& filepath);
        static std::unordered_map<GLenum, std::string> preProcess(const std::string& source);

        void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    };
}
