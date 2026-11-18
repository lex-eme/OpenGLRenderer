#pragma once

#include "core/KeyCodes.h"

namespace Bottleneck {
    class Input {
    public:
        static bool IsKeyPressed(const KeyCode keycode) { return s_instance->IsKeyPressedImpl(keycode); }

        static bool IsMouseButtonPressed(const int button) { return s_instance->IsMouseButtonPressedImpl(button); }
        static std::pair<float, float> GetMousePosition() { return s_instance->GetMousePositionImpl(); }
        static float GetMouseX() { return s_instance->GetMouseXImpl(); }
        static float GetMouseY() { return s_instance->GetMouseYImpl(); }

    protected:
        ~Input() = default;
        virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

    private:
        static Input* s_instance;
    };
}
