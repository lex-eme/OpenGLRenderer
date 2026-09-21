#include "ShaderProgram.h"

#include <fstream>
#include <iostream>

bool checkOpenGLError() {
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
    glUseProgram(ID);
}

void ShaderProgram::setFloatUniform(const std::string& name, const float value) const {
    const GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "Shader uniform '" + name << "' does not exist" << std::endl;
    }
    glProgramUniform1f(ID, location, value);
    checkOpenGLError();
    printProgramLog(ID);
}

ShaderProgram ShaderProgram::createShaderProgram(const std::string& vertexShaderSourcePath,
                                                 const std::string& fragmentShaderSourcePath) {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    const GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    const std::string vertShaderStr = readShaderSource(vertexShaderSourcePath);
    const std::string fragShaderStr = readShaderSource(fragmentShaderSourcePath);
    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vShader, 1, &vertShaderSrc, nullptr);
    glShaderSource(fShader, 1, &fragShaderSrc, nullptr);
    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        std::cerr << "Vertex shader compilation failed" << std::endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        std::cerr << "Fragment shader compilation failed" << std::endl;
        printShaderLog(fShader);
    }

    const GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        std::cerr << "Shader program linking failed" << std::endl;
        printProgramLog(vfProgram);
    }

    return ShaderProgram(vfProgram);
}

ShaderProgram::ShaderProgram(const GLuint ID)
    : ID(ID){
}

std::string ShaderProgram::readShaderSource(const std::string& filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    std::string line;
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

void ShaderProgram::printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*) malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        std::cout << "Shader Info Log: " << log << std::endl;
        free(log);
    }
}

void ShaderProgram::printProgramLog(GLuint program) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*) malloc(len);
        glGetProgramInfoLog(program, len, &chWrittn, log);
        std::cout << "Program Info Log: " << log << std::endl;
        free(log);
    }
}
