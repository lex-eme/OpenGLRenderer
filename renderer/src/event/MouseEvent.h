#pragma once
#include <sstream>

#include "Event.h"
#include "core/MouseCodes.h"


class MouseMovedEvent final : public Event {
    float m_mouseX;
    float m_mouseY;

public:
    MouseMovedEvent(const float x, const float y)
        : m_mouseX(x), m_mouseY(y) {
    }

    [[nodiscard]] float getX() const { return m_mouseX; }
    [[nodiscard]] float getY() const { return m_mouseY; }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

class MouseScrolledEvent final : public Event {
    float m_xOffset;
    float m_yOffset;

public:
    MouseScrolledEvent(const float xOffset, const float yOffset)
        : m_xOffset(xOffset), m_yOffset(yOffset) {
    }

    [[nodiscard]] float getXOffset() const { return m_xOffset; }
    [[nodiscard]] float getYOffset() const { return m_yOffset; }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

class MouseButtonEvent : public Event {
protected:
    MouseCode m_button;

public:
    [[nodiscard]] MouseCode GetMouseButton() const { return m_button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

protected:
    explicit MouseButtonEvent(const MouseCode button)
        : m_button(button) {
    }
};

class MouseButtonPressedEvent final : public MouseButtonEvent {
public:
    explicit MouseButtonPressedEvent(const MouseCode button)
        : MouseButtonEvent(button) {
    }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent final : public MouseButtonEvent {
public:
    explicit MouseButtonReleasedEvent(const MouseCode button)
        : MouseButtonEvent(button) {
    }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};
