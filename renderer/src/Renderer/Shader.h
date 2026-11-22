#pragma once
#include <string>
#include <unordered_map>

#include "core/Core.h"

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

        [[nodiscard]] virtual const std::string& getName() const = 0;

        static Ref<Shader> Create(const std::string& filepath);
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    };

    class ShaderLibrary {
        std::unordered_map<std::string, Ref<Shader>> m_shaders;

    public:
        void add(const std::string& name, const Ref<Shader>& shader);
        void add(const Ref<Shader>& shader);

        Ref<Shader> load(const std::string& filepath);
        Ref<Shader> load(const std::string& name, const std::string& filepath);

        Ref<Shader> get(const std::string& name);
    };
}
