#include "Sampler.h"

#include <glad/glad.h>


Sampler::Sampler() {
    glGenSamplers(1, &id);
}

void Sampler::bind(const unsigned int texUnit) const {
    if (texUnit < GL_MAX_TEXTURE_IMAGE_UNITS) {
        glBindSampler(texUnit, id);
    }
}

void Sampler::setWrapS(WrapType type) const {
    glSamplerParameteri(id, GL_TEXTURE_WRAP_S, static_cast<GLint>(type));
}

void Sampler::setWrapT(WrapType type) const {
    glSamplerParameteri(id, GL_TEXTURE_WRAP_T, static_cast<GLint>(type));
}

void Sampler::setWrapR(WrapType type) const {
    glSamplerParameteri(id, GL_TEXTURE_WRAP_R, static_cast<GLint>(type));
}

void Sampler::setMinFilter(FilterType type) const {
    glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(type));
}

void Sampler::setMagFilter(FilterType type) const {
    glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(type));
}

void Sampler::setMinMipmapFilter(MipmapFilterType type) const {
    glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(type));
}

void Sampler::setAnisotropicFilter(const float level) const {
    if (GL_EXT_texture_filter_anisotropic) {
        float maxAnisotropy = 1.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glSamplerParameterf(id, GL_TEXTURE_MAX_ANISOTROPY_EXT, glm::min(level, maxAnisotropy));
    }
}

void Sampler::setBorderColor(const glm::vec4 color) const {
    glSamplerParameterfv(id, GL_TEXTURE_BORDER_COLOR, &color.r);
}
