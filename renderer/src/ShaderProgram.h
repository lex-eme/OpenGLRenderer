#pragma once
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"


class ShaderProgram {
public:
    GLuint ID;

public:
    ShaderProgram() = default;

    void setActive() const;
    void setFloatUniform(const std::string& name, float value) const;
    void setMat4Uniform(const std::string& name, const glm::mat4& value) const;

    static ShaderProgram createShaderProgram(const std::string& vertexShaderSourcePath, const std::string& fragmentShaderSourcePath);

private:
    explicit ShaderProgram(GLuint ID);
    GLint getUniformLocation(const std::string& name) const;

    static std::string readShaderSource(const std::string& filePath);
    static void printShaderLog(GLuint shader);
    static void printProgramLog(GLuint program);
};
