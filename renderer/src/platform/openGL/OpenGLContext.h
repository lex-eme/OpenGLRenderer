#pragma once
#include "Bottleneck/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Bottleneck {
    class OpenGLContext final : public GraphicsContext {
        GLFWwindow* m_windowHandle;
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);

        void init() override;
        void swapBuffers() override;
    };
}
