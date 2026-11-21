#include "pch.h"

#include "OpenGLShader.h"

namespace Bottleneck {
    static GLenum shaderTypeFromString(const std::string& type) {
        if (type == "vertex") {
            return GL_VERTEX_SHADER;
        }
        if (type == "fragment" || type == "pixel") {
            return GL_FRAGMENT_SHADER;
        }

        BN_CORE_ERROR("Unknown shader type '{0}'", type);
        assert(false);
    }

    OpenGLShader::OpenGLShader(const std::string& filepath) {
        const std::string source = readFile(filepath);
        const auto shaderSources = preProcess(source);
        compile(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexSrc;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
        compile(shaderSources);
    }

    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_rendererID);
    }

    void OpenGLShader::bind() const {
        glUseProgram(m_rendererID);
    }

    void OpenGLShader::unbind() const {
        glUseProgram(0);
    }

    void OpenGLShader::uploadUniformInt(const std::string& name, const int value) {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glProgramUniform1i(m_rendererID, location, value);
    }

    void OpenGLShader::uploadUniformFloat(const std::string& name, const float value) {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glProgramUniform1f(m_rendererID, location, value);
    }

    void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& value) {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glProgramUniform2fv(m_rendererID, location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& value) {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glProgramUniform3fv(m_rendererID, location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& value) {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glProgramUniform4fv(m_rendererID, location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glProgramUniformMatrix3fv(m_rendererID, location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glProgramUniformMatrix4fv(m_rendererID, location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    std::string OpenGLShader::readFile(const std::string& filepath) {
        std::ifstream in(filepath, std::ios::in | std::ios::binary);

        std::string result;
        if (in) {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        } else {
            BN_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source) {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        std::string::size_type pos = source.find(typeToken, 0);
        while (pos != std::string::npos) {
            const size_t eol = source.find_first_of("\r\n", pos);
            if (eol == std::string::npos) {
                BN_CORE_ERROR("Syntax error");
                assert(false);
            }
            const size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            GLenum shaderType = shaderTypeFromString(type);

            const size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[shaderType] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
        const GLuint program = glCreateProgram();
        std::vector<GLenum> glShaderIDs(shaderSources.size());

        for (const auto& [type, source]: shaderSources) {
            const GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE) {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);
                glDeleteProgram(program);

                BN_CORE_ERROR("{0}", infoLog.data());
                assert(false);
            }

            glAttachShader(program, shader);
            glShaderIDs.push_back(shader);
        }

        m_rendererID = program;

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*) &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            for (const auto id: glShaderIDs) {
                glDeleteShader(id);
            }

            BN_CORE_ERROR("{0}", infoLog.data());
            assert(false);
        }

        for (const auto id: glShaderIDs) {
            glDetachShader(program, id);
        }
    }
}
