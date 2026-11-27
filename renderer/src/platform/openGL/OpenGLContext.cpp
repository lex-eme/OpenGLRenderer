#include "pch.h"

#include "OpenGLContext.h"

namespace Bottleneck {
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_windowHandle(windowHandle) {
    }

    void OpenGLContext::init() {
        BN_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_windowHandle);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            BN_CORE_ERROR("Failed to initialize GLAD");
            assert(0);
        }

        BN_CORE_INFO("OpenGL info:");
        BN_CORE_INFO("\tVendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        BN_CORE_INFO("\tRenderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        BN_CORE_INFO("\tVersion: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        BN_CORE_INFO("\tGLSL version: {0}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    }

    void OpenGLContext::swapBuffers() {
        BN_PROFILE_FUNCTION();

        glfwSwapBuffers(m_windowHandle);
    }
}
