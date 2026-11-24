#pragma once
#include <sstream>

#include "Event.h"
#include "Bottleneck/Core/KeyCodes.h"

class KeyEvent : public Event {
protected:
    KeyCode m_keyCode;

public:
    [[nodiscard]] KeyCode getKeyCode() const { return m_keyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
    explicit KeyEvent(const KeyCode keycode)
        : m_keyCode(keycode) {
    }
};

class KeyPressedEvent final : public KeyEvent {
public:
    explicit KeyPressedEvent(const KeyCode keycode)
        : KeyEvent(keycode) {
    }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_keyCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
};

class KeyReleasedEvent final : public KeyEvent {
public:
    explicit KeyReleasedEvent(const KeyCode keycode)
        : KeyEvent(keycode) {
    }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_keyCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};
