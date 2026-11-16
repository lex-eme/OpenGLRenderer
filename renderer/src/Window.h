#pragma once
#include <functional>

#include "event/Event.h"

namespace Bottleneck {
    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;

        explicit WindowProps(const std::string& title = "Renderer", const uint32_t width = 1280,
                             const uint32_t height = 720)
            : title(title), width(width), height(height) {
        }
    };

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        [[nodiscard]] virtual unsigned int getWidth() const = 0;
        [[nodiscard]] virtual unsigned int getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        [[nodiscard]] virtual bool isVSync() const = 0;

        [[nodiscard]] virtual void* getNativeWindow() const = 0;

        static Window* create(const WindowProps& props = WindowProps());
    };
}