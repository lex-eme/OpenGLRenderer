#pragma once

#include <memory>

#include "LayerStack.h"
#include "Window.h"
#include "event/ApplicationEvent.h"
#include "event/KeyEvent.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"


namespace Bottleneck {
    class ImGuiLayer;

    class Application {
        std::unique_ptr<Window> m_window;
        LayerStack m_layerStack;
        bool m_running = true;
        ImGuiLayer* m_imGuiLayer;

        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<VertexArray> m_vertexArray;

        static Application* s_instance;

    public:
        Application();
        virtual ~Application() = default;

        void run();
        void onEvent(Event& event);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        [[nodiscard]] Window& getWindow() const {
            return *m_window;
        }

        static Application& get() {
            return *s_instance;
        }

    private:
        bool onWindowClose(WindowCloseEvent& event);
        bool onKeyPress(const KeyPressedEvent& event);
    };

    // To be defined in client application
    Application* CreateApplication();
}
