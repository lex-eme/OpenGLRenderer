#include "Texture.h"

#include <iostream>

#include "Image.h"
#include "glad/glad.h"

void Texture::loadFromFile(const std::string& inPath) {
    if (Image image; image.loadFromFile(inPath)) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (GL_EXT_texture_filter_anisotropic) {
            std::cout << "Anisotropic filtering extension supported" << std::endl;
            GLfloat anisoSetting = 0.0f;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
        }

        GLenum format;
        if (image.nrChannels == 1) {
            format = GL_RED;
        } else if (image.nrChannels == 3) {
            format = GL_RGB;
        } else {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture::unload() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &id);
    id = 0;
}
