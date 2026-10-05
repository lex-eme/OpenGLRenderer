#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>

enum class WrapType : int {
    Repeat = GL_REPEAT,
    MirrorRepeat = GL_MIRRORED_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER,
};

enum class FilterType : int {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
};

enum class MipmapFilterType : int {
    NearestNearest = GL_NEAREST_MIPMAP_NEAREST,
    LinearNearest = GL_LINEAR_MIPMAP_NEAREST,
    NearestLinear = GL_NEAREST_MIPMAP_LINEAR,
    LinearLinear = GL_LINEAR_MIPMAP_LINEAR,
};

class Sampler {
    unsigned int id{};

public:
    Sampler();

    void bind(unsigned int texUnit) const;
    void setWrapS(WrapType type) const;
    void setWrapT(WrapType type) const;
    void setWrapR(WrapType type) const;
    void setMinFilter(FilterType type) const;
    void setMagFilter(FilterType type) const;
    void setMinMipmapFilter(MipmapFilterType type) const;
    void setAnisotropicFilter(float level) const;
    void setBorderColor(glm::vec4 color) const;
};
