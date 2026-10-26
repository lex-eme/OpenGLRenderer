#pragma once
#include <string>

#include "glm/glm.hpp"


class ShaderProgram {
public:
    unsigned int id;

public:
    ShaderProgram() = default;

    void setActive() const;
    void setFloatUniform(const std::string& name, float value) const;
    void setIntUniform(const std::string& name, int value) const;
    void setVec3Uniform(const std::string& name, const glm::vec3& value) const;
    void setMat4Uniform(const std::string& name, const glm::mat4& value) const;
    void bindUniformBufferObject(const std::string& name, unsigned int bindingPoint) const;

    static ShaderProgram createShaderProgram(const std::string& vertexShaderSourcePath,
                                             const std::string& fragmentShaderSourcePath);

protected:
    explicit ShaderProgram(unsigned int ID);

    [[nodiscard]] int getUniformLocation(const std::string& name) const;
    static bool checkOpenGLError();
    static std::string readShaderSource(const std::string& filePath);
    static void printShaderLog(unsigned int shader);
    static void printProgramLog(unsigned int program);
};
