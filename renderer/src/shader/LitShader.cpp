#include "../pch.h"

#include "LitShader.h"


void LitShader::setDirectionalLight(const DirectionalLight& dirLight) const {
    setVec3Uniform("dirLight.direction", dirLight.direction);
    setVec3Uniform("dirLight.ambient", dirLight.ambient);
    setVec3Uniform("dirLight.diffuse", dirLight.diffuse);
    setVec3Uniform("dirLight.specular", dirLight.specular);
}

void LitShader::setPointLights(const std::vector<PointLight>& pointLights) const {
    const int max = MAX_POINT_LIGHT_COUNT < pointLights.size() ? MAX_POINT_LIGHT_COUNT : pointLights.size();
    for (int i = 0; i < max; i += 1) {
        const PointLight& light = pointLights[i];
        std::string name = std::format("pointLights[{0}].", i);
        setVec3Uniform(name + "position", light.position);
        setVec3Uniform(name + "ambient", light.ambient);
        setVec3Uniform(name + "diffuse", light.diffuse);
        setVec3Uniform(name + "specular", light.specular);
        setFloatUniform(name + "constant", light.constant);
        setFloatUniform(name + "linear", light.linear);
        setFloatUniform(name + "quadratic", light.quadratic);
    }
}

void LitShader::setSpotLights(const std::vector<SpotLight>& spotLights) const {
    const int max = MAX_SPOT_LIGHT_COUNT < spotLights.size() ? MAX_SPOT_LIGHT_COUNT : spotLights.size();
    for (int i = 0; i < max; i += 1) {
        const SpotLight& light = spotLights[i];
        std::string name = std::format("spotLights[{0}].", i);
        setVec3Uniform(name + "position", light.position);
        setVec3Uniform(name + "direction", light.direction);
        setVec3Uniform(name + "ambient", light.ambient);
        setVec3Uniform(name + "diffuse", light.diffuse);
        setVec3Uniform(name + "specular", light.specular);
        setFloatUniform(name + "constant", light.constant);
        setFloatUniform(name + "linear", light.linear);
        setFloatUniform(name + "quadratic", light.quadratic);
        setFloatUniform(name + "cutOff", light.innerCutOff);
        setFloatUniform(name + "outerCutOff", light.outerCutOff);
    }
}

LitShader LitShader::createShaderProgram(const std::string& vertexShaderSourcePath,
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
        exit(-1);
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return LitShader(vfProgram);
}

LitShader::LitShader(const unsigned int ID)
    : ShaderProgram(ID) {
}
