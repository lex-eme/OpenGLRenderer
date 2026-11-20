#pragma once
#include <string>

namespace Bottleneck {
    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void uploadUniformInt(const std::string& name, int value) = 0;

        virtual void uploadUniformFloat(const std::string& name, float value) = 0;
        virtual void uploadUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void uploadUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void uploadUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

        virtual void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
        virtual void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;

        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}
