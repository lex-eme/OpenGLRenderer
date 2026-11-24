#pragma once
#include <glm/glm.hpp>

#include "VertexArray.h"
#include "core/Core.h"

namespace Bottleneck {
    class RendererAPI {
    public:
        enum class API {
            None = 0, OpenGL = 1
        };

    private:
        inline static API s_API = API::OpenGL;

    public:
        virtual ~RendererAPI() = default;

        virtual void init() = 0;
        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void setClearColor(const glm::vec4& color) = 0;
        virtual void clear() = 0;

        virtual void drawIndexed(const Ref<VertexArray>& vertexArray) = 0;

        static API GetAPI() { return s_API; }
    };
}
