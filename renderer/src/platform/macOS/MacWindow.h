#pragma once

#include "../../core/Window.h"
#include "GLFW/glfw3.h"
#include "Renderer/GraphicsContext.h"


namespace Bottleneck {
    class MacWindow final : public Window {
        struct WindowData {
            std::string title;
            unsigned int width, height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        GLFWwindow* m_window{};
        GraphicsContext* m_context{};
        WindowData m_data{};

    public:
        explicit MacWindow(const WindowProps& props);
        ~MacWindow() override;

        void onUpdate() override;

        [[nodiscard]] unsigned getWidth() const override {
            return m_data.width;
        }

        [[nodiscard]] unsigned getHeight() const override {
            return m_data.height;
        }

        void setEventCallback(const EventCallbackFn& callback) override;
        void setVSync(bool enabled) override;

        [[nodiscard]] bool isVSync() const override {
            return m_data.VSync;
        }

        [[nodiscard]] void* getNativeWindow() const override { return m_window; }

    private:
        void init(const WindowProps& props);
        void shutdown() const;
    };
}