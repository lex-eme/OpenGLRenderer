#pragma once

#include <memory>

#include "Bottleneck/Core/LayerStack.h"
#include "Bottleneck/Core/Window.h"
#include "Bottleneck/Event/ApplicationEvent.h"
#include "Bottleneck/Event/KeyEvent.h"


namespace Bottleneck {
    class ImGuiLayer;

    class Application {
        std::unique_ptr<Window> m_window;
        LayerStack m_layerStack;
        bool m_running = true;
        bool m_minimized = false;
        ImGuiLayer* m_imGuiLayer;
        float m_lastFrameTime;

        static Application* s_instance;

    public:
        Application();
        virtual ~Application();

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
        bool onWindowResize(const WindowResizeEvent& event);
        bool onKeyPress(const KeyPressedEvent& event);
    };

    // To be defined in client application
    Application* CreateApplication();
}
