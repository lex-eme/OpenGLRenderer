#include "pch.h"

#include "Application.h"

#include "Timestep.h"
#include "Bottleneck/Core/Core.h"
#include "GLFW/glfw3.h"

#include "Bottleneck/Imgui/ImGuiLayer.h"
#include "Bottleneck/Renderer/Renderer.h"

namespace Bottleneck {
    Application* Application::s_instance = nullptr;

    Application::Application() {
        BN_PROFILE_FUNCTION();

        assert(s_instance == nullptr);
        s_instance = this;

        const WindowProps props("The new name", 720, 720);
        m_window = Window::Create(props);
        m_window->setEventCallback(BN_BIND_EVENT_FN(Application::onEvent));

        Renderer::Init();

        m_imGuiLayer = new ImGuiLayer();
        pushOverlay(m_imGuiLayer);
    }

    Application::~Application() {
        BN_PROFILE_FUNCTION();

        Renderer::Shutdown();
    }

    void Application::run() {
        BN_PROFILE_FUNCTION();

        while (m_running) {
            BN_PROFILE_SCOPE("RunLoop");
            // should be in a class like Platform::GetTime() which should have a MacPlatform::GetTime()
            const float time = static_cast<float>(glfwGetTime());
            const Timestep timestep(time - m_lastFrameTime);
            m_lastFrameTime = time;

            if (!m_minimized) {
                {
                    BN_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (Layer* layer: m_layerStack) {
                        layer->onUpdate(timestep);
                    }
                }
            }

            m_imGuiLayer->begin(); {
                BN_PROFILE_SCOPE("LayerStack onImGuiRender");

                for (Layer* layer: m_layerStack) {
                    layer->onImGuiRender();
                }
            }
            m_imGuiLayer->end();

            m_window->onUpdate();
        }
    }

    void Application::onEvent(Event& event) {
        BN_PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>(BN_BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.dispatch<WindowResizeEvent>(BN_BIND_EVENT_FN(Application::onWindowResize));
        dispatcher.dispatch<KeyPressedEvent>(BN_BIND_EVENT_FN(Application::onKeyPress));

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it += 1) {
            if (event.handled) {
                break;
            }
            (*it)->onEvent(event);
        }
    }

    void Application::pushLayer(Layer* layer) {
        BN_PROFILE_FUNCTION();

        m_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* layer) {
        BN_PROFILE_FUNCTION();

        m_layerStack.pushOverlay(layer);
        layer->onAttach();
    }

    bool Application::onWindowClose(WindowCloseEvent& event) {
        m_running = false;
        return true;
    }

    bool Application::onWindowResize(const WindowResizeEvent& event) {
        BN_PROFILE_FUNCTION();

        if (event.getWidth() == 0 || event.getHeight() == 0) {
            m_minimized = true;
            return false;
        }

        m_minimized = false;
        Renderer::OnWindowResize(event.getWidth(), event.getHeight());

        return false;
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
