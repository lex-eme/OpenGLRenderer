#include "pch.h"

#include "MacWindow.h"

#include "event/ApplicationEvent.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"
#include "platform/openGL/OpenGLContext.h"

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(const int error, const char* description) {
    BN_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

namespace Bottleneck {
    Window* Window::create(const WindowProps& props) {
        return new MacWindow(props);
    }

    MacWindow::MacWindow(const WindowProps& props) {
        init(props);
    }

    MacWindow::~MacWindow() {
        shutdown();
    }

    void MacWindow::onUpdate() {
        glfwPollEvents();
        m_context->swapBuffers();
    }

    void MacWindow::setEventCallback(const EventCallbackFn& callback) {
        m_data.EventCallback = callback;
    }

    void MacWindow::setVSync(const bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        m_data.VSync = enabled;
    }

    void MacWindow::init(const WindowProps& props) {
        BN_CORE_INFO("Initialization of MacOS window");
        m_data.title = props.title;
        m_data.width = props.width;
        m_data.height = props.height;

        if (s_GLFWWindowCount == 0) {
            const int success = glfwInit();
            assert(success);
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Apple only (MacWindow)

        m_window = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), m_data.title.c_str(),
                                    nullptr, nullptr);

        if (m_window == nullptr) {
            BN_CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
            assert(0);
        }

        m_context = new OpenGLContext(m_window);
        m_context->init();

        s_GLFWWindowCount += 1;
        glfwSetWindowUserPointer(m_window, &m_data);
        setVSync(true);

        BN_CORE_INFO("OpenGL version: {0}.{1}", GLVersion.major, GLVersion.minor);

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, const int key, int scancode, int action, int mods) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    break;
                }
                default: break;
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                default: break;
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, const double xOffset, const double yOffset) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, const double xPos, const double yPos) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            data.EventCallback(event);
        });
    }

    void MacWindow::shutdown() const {
        glfwDestroyWindow(m_window);
        s_GLFWWindowCount -= 1;

        if (s_GLFWWindowCount == 0) {
            glfwTerminate();
        }
    }
}
