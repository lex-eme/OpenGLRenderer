#pragma once

#include "Bottleneck/Core/Input.h"
#include "Bottleneck/Core/KeyCodes.h"

namespace Bottleneck {
    class MacInput final : public Input {
    protected:
        bool IsKeyPressedImpl(KeyCode keycode) override;
        bool IsMouseButtonPressedImpl(int button) override;
        std::pair<float, float> GetMousePositionImpl() override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
    };
}
