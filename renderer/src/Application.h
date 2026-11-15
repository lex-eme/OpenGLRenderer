#pragma once

#include <memory>

#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "event/ApplicationEvent.h"
#include "event/KeyEvent.h"


namespace Bottleneck {
    class ImGuiLayer;

    class BOTTLENECK_API Application {
    private:
        std::unique_ptr<Window> m_window;
        LayerStack m_layerStack;
        bool m_running = true;
        ImGuiLayer* m_imGuiLayer;

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
        bool onKeyPress(const KeyPressedEvent& event);
    };

    // To be defined in client
    Application* CreateApplication();
}
