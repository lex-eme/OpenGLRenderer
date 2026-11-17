#pragma once

namespace Bottleneck {
    enum class RenderAPI {
        None = 0,
        OpenGL = 1
    };

    class Renderer {
        inline static auto s_renderAPI = RenderAPI::OpenGL;

    public:
        static RenderAPI GetAPI() {
            return s_renderAPI;
        }
    };
}
