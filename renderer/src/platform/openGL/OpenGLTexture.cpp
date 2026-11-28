#include "pch.h"

#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Bottleneck {
    OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height)
        : m_width(width), m_height(height) {
        BN_PROFILE_FUNCTION();

        m_internalFormat = GL_RGBA8;
        m_dataFormat = GL_RGBA;

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, nullptr);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_path(path) {
        BN_PROFILE_FUNCTION();

        int width;
        int height;
        int channels;

        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr; {
            BN_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        assert(data != nullptr);
        m_width = width;
        m_height = height;

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (GLAD_GL_EXT_texture_filter_anisotropic) {
            GLfloat anisoSetting = 0.0f;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
        }

        GLint internalFormat;
        GLenum dataFormat;
        if (channels == 1) {
            internalFormat = GL_RED;
            dataFormat = GL_RED;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        } else {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }

        m_internalFormat = internalFormat;
        m_dataFormat = dataFormat;

        assert(internalFormat != 0 && dataFormat != 0);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        BN_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::setData(void* data, const uint32_t size) {
        const uint32_t bytesPerPixel = m_dataFormat == GL_RGBA ? 4 : 3;
        assert(size == (m_width * m_height * bytesPerPixel));
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::bind(const uint32_t slot) const {
        BN_PROFILE_FUNCTION();

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }
}
