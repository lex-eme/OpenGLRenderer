#include "pch.h"

#include "OpenGLShader.h"

namespace Bottleneck {
    OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
        // Create an empty vertex shader handle
        const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar* source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            BN_CORE_ERROR("{0}", infoLog.data());
            assert(false);
            return;
        }

        // Create an empty fragment shader handle
        const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            BN_CORE_ERROR("{0}", infoLog.data());
            assert(false);
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        m_rendererID = glCreateProgram();
        const GLuint program = m_rendererID;

        // Attach our shaders to our program
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

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
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            BN_CORE_ERROR("{0}", infoLog.data());
            assert(false);
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
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
}
