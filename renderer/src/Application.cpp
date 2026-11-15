#include "pch.h"

#include "Application.h"
#include "Core.h"

#include "imgui/ImGuiLayer.h"

namespace Bottleneck {
    Application* Application::s_instance = nullptr;

    Application::Application() {
        assert(s_instance == nullptr);
        s_instance = this;
        m_window = std::unique_ptr<Window>(Window::create());
        m_window->setEventCallback(BN_BIND_EVENT_FN(Application::onEvent));

        m_imGuiLayer = new ImGuiLayer();
        pushOverlay(m_imGuiLayer);
    }

    Application::~Application() {
    }

    void Application::run() {
        while (m_running) {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer: m_layerStack) {
                layer->onUpdate();
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
