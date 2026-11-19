#include "../pch.h"

#include "Application.h"

#include <memory>

#include "TimeStep.h"
#include "core/Core.h"
#include "GLFW/glfw3.h"

#include "../imgui/ImGuiLayer.h"

namespace Bottleneck {
    Application* Application::s_instance = nullptr;

    Application::Application() {
        assert(s_instance == nullptr);
        s_instance = this;

        WindowProps props("The new name", 720, 720);
        m_window = std::unique_ptr<Window>(Window::create(props));
        m_window->setEventCallback(BN_BIND_EVENT_FN(Application::onEvent));
        m_window->setVSync(false);

        m_imGuiLayer = new ImGuiLayer();
        pushOverlay(m_imGuiLayer);
    }

    void Application::run() {
        while (m_running) {
            const float time = static_cast<float>(glfwGetTime()); // should be in a class like Platform::GetTime() which should have a MacPlatform::GetTime()
            TimeStep timeStep = time - m_lastFrameTime;
            m_lastFrameTime = time;

            for (Layer* layer: m_layerStack) {
                layer->onUpdate(timeStep);
            }

            m_imGuiLayer->begin();
            for (Layer* layer: m_layerStack) {
                layer->onImGuiRender();
            }
            m_imGuiLayer->end();

            m_window->onUpdate();
        }
    }

    void Application::onEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BN_BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(BN_BIND_EVENT_FN(Application::onKeyPress));

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it += 1) {
            if (event.handled) {
                break;
            }
            (*it)->onEvent(event);
        }
    }

    void Application::pushLayer(Layer* layer) {
        m_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* layer) {
        m_layerStack.pushOverlay(layer);
        layer->onAttach();
    }

    bool Application::onWindowClose(WindowCloseEvent& event) {
        m_running = false;
        return true;
    }

    bool Application::onKeyPress(const KeyPressedEvent& event) {
        switch (event.getKeyCode()) {
            case Key::Escape:
                m_running = false;
                return true;
            default: break;
        }

        return false;
    }
}
