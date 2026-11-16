#include "pch.h"

#include "OpenGLContext.h"

namespace Bottleneck {
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_windowHandle(windowHandle) {
    }

    void OpenGLContext::init() {
        glfwMakeContextCurrent(m_windowHandle);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            BN_CORE_ERROR("Failed to initialize GLAD");
            assert(0);
        }
    }

    void OpenGLContext::swapBuffers() {
        glfwSwapBuffers(m_windowHandle);
    }
}
