#include "pch.h"

#include "MacInput.h"

#include "Bottleneck/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Bottleneck {
    Input* Input::s_instance = new MacInput();

    bool MacInput::IsKeyPressedImpl(const KeyCode keycode) {
        const auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        const int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS;
    }

    bool MacInput::IsMouseButtonPressedImpl(const int button) {
        const auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        const int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> MacInput::GetMousePositionImpl() {
        const auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return {static_cast<float>(xPos), static_cast<float>(yPos)};
    }

    float MacInput::GetMouseXImpl() {
        auto [x, y] = GetMousePositionImpl();
        return x;
    }

    float MacInput::GetMouseYImpl() {
        auto [x, y] = GetMousePositionImpl();
        return y;
    }
}
