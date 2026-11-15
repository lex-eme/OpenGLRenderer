#pragma once
#include <string>


enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowLostFocus, WindowMoved,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory {
    None = 0,
    EventCategoryApplication = 1 << 0,
    EventCategoryInput = 1 << 1,
    EventCategoryKeyboard = 1 << 2,
    EventCategoryMouse = 1 << 3,
    EventCategoryMouseButton = 1 << 4
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
virtual EventType getEventType() const override { return GetStaticType(); }\
virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

class Event {
    friend class EventDispatcher;

public:
    bool handled = false;

public:
    virtual ~Event() = default;

    [[nodiscard]] virtual EventType getEventType() const = 0;
    [[nodiscard]] virtual const char* getName() const = 0;
    [[nodiscard]] virtual int getCategoryFlags() const = 0;
    [[nodiscard]] virtual std::string toString() const { return getName(); }

    [[nodiscard]] bool isInCategory(const EventCategory category) const {
        return getCategoryFlags() & category;
    }
};

class EventDispatcher {
    Event& m_Event;

public:
    explicit EventDispatcher(Event& event)
        : m_Event(event) {
    }

    // F will be deduced by the compiler
    template<typename T, typename F>
    bool Dispatch(const F& func) {
        if (m_Event.getEventType() == T::GetStaticType()) {
            m_Event.handled |= func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
    return os << e.toString();
}
