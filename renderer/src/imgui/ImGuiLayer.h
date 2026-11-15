#pragma once
#include "Layer.h"


namespace Bottleneck {
    class ImGuiLayer final : public Layer {
        bool m_blockEvents = true;

    public:
        void onAttach() override;
        void onDetach() override;
        void onEvent(Event& event) override;
        void onImGuiRender() override;

        void begin();
        void end();

        void blockEvents(const bool block) {
            m_blockEvents = block;
        }
    };
}
