#pragma once
#include <sstream>

#include "Event.h"


class WindowResizeEvent final : public Event {
    unsigned int m_width;
    unsigned int m_height;

public:
    WindowResizeEvent(const unsigned int width, const unsigned int height)
        : m_width(width), m_height(height) {
    }

    [[nodiscard]] unsigned int getWidth() const { return m_width; }
    [[nodiscard]] unsigned int getHeight() const { return m_height; }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_width << ", " << m_height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class WindowCloseEvent final : public Event
{
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
