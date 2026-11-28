#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "Bottleneck/Renderer/Shader.h"

namespace Bottleneck {
    class OpenGLShader final : public Shader {
        uint32_t m_rendererID;
        std::string m_name;

    public:
        explicit OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader() override;

        void bind() const override;
        void unbind() const override;

        void setInt(const std::string& name, int value) override { uploadUniformInt(name, value); }
        void setFloat(const std::string& name, float value) override { uploadUniformFloat(name, value); }
        void setFloat3(const std::string& name, const glm::vec3& value) override { uploadUniformFloat3(name, value); }
        void setFloat4(const std::string& name, const glm::vec4& value) override { uploadUniformFloat4(name, value); }
        void setMat4(const std::string& name, const glm::mat4& value) override { uploadUniformMat4(name, value); }

        void uploadUniformInt(const std::string& name, int value) override;

        void uploadUniformFloat(const std::string& name, float value);
        void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void uploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

        [[nodiscard]] const std::string& getName() const override { return m_name; }

    private:
        static std::string readFile(const std::string& filepath);
        static std::unordered_map<unsigned int, std::string> preProcess(const std::string& source);

        void compile(const std::unordered_map<unsigned int, std::string>& shaderSources);
    };
}
