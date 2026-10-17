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
    void setIntUniform(const std::string& name, int value) const;
    void setVec3Uniform(const std::string& name, const glm::vec3& value) const;
    void setMat4Uniform(const std::string& name, const glm::mat4& value) const;

    static ShaderProgram createShaderProgram(const std::string& vertexShaderSourcePath,
                                             const std::string& fragmentShaderSourcePath);

protected:
    explicit ShaderProgram(GLuint ID);

    GLint getUniformLocation(const std::string& name) const;
    static bool checkOpenGLError();
    static std::string readShaderSource(const std::string& filePath);
    static void printShaderLog(GLuint shader);
    static void printProgramLog(GLuint program);
};
