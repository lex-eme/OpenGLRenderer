#include "ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

bool ShaderProgram::checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        std::cerr << "glError: " << glErr << std::endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

void ShaderProgram::setActive() const {
    glUseProgram(id);
}

void ShaderProgram::setFloatUniform(const std::string& name, const float value) const {
    const GLint location = getUniformLocation(name);
    glProgramUniform1f(id, location, value);
    checkOpenGLError();
    printProgramLog(id);
}

void ShaderProgram::setIntUniform(const std::string& name, const int value) const {
    const GLint location = getUniformLocation(name);
    glProgramUniform1i(id, location, value);
    checkOpenGLError();
    printProgramLog(id);
}

void ShaderProgram::setVec3Uniform(const std::string& name, const glm::vec3& value) const {
    const GLint location = getUniformLocation(name);
    glProgramUniform3fv(id, location, 1, glm::value_ptr(value));
    checkOpenGLError();
    printProgramLog(id);
}

void ShaderProgram::setMat4Uniform(const std::string& name, const glm::mat4& value) const {
    const GLint location = getUniformLocation(name);
    glProgramUniformMatrix4fv(id, location, 1, GL_FALSE, glm::value_ptr(value));
    checkOpenGLError();
    printProgramLog(id);
}

void ShaderProgram::bindUniformBufferObject(const std::string& name, const unsigned int bindingPoint) const {
    const unsigned int index = glGetUniformBlockIndex(id, name.c_str());
    glUniformBlockBinding(id, index, bindingPoint);
    checkOpenGLError();
    printProgramLog(id);
}

ShaderProgram ShaderProgram::createShaderProgram(const std::string& vertexShaderSourcePath,
                                                 const std::string& fragmentShaderSourcePath) {
    GLint success;

    const std::string vertShaderStr = readShaderSource(vertexShaderSourcePath);
    const std::string fragShaderStr = readShaderSource(fragmentShaderSourcePath);
    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    const GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertShaderSrc, nullptr);
    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (success != 1) {
        std::cerr << "Vertex shader compilation failed" << std::endl;
        printShaderLog(vShader);
        exit(-1);
    }

    const GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragShaderSrc, nullptr);
    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (success != 1) {
        std::cerr << "Fragment shader compilation failed" << std::endl;
        printShaderLog(fShader);
        exit(-1);
    }

    const GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &success);
    if (success != 1) {
        std::cerr << "Shader program linking failed" << std::endl;
        printProgramLog(vfProgram);
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return ShaderProgram(vfProgram);
}

ShaderProgram::ShaderProgram(const unsigned int ID)
    : id(ID) {
}

std::string ShaderProgram::readShaderSource(const std::string& filePath) {
    std::string content;

    std::ifstream file;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(filePath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << file.rdbuf();
        file.close();
        content = vShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    return content;
}

void ShaderProgram::printShaderLog(const unsigned int shader) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = static_cast<char*>(malloc(len));
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        std::cout << "Shader Info Log: " << log << std::endl;
        free(log);
    }
}

void ShaderProgram::printProgramLog(const unsigned int program) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = static_cast<char*>(malloc(len));
        glGetProgramInfoLog(program, len, &chWrittn, log);
        std::cout << "Program Info Log: " << log << std::endl;
        free(log);
    }
}

int ShaderProgram::getUniformLocation(const std::string& name) const {
    const GLint location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        std::cerr << "Shader uniform '" + name << "' does not exist" << std::endl;
    }
    return location;
}
