#pragma once

#include <string>

#include "Timestep.h"
#include "../Event/Event.h"

namespace Bottleneck {
    class Layer {
    protected:
        std::string m_debugName;

    public:
        explicit Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate(Timestep timestep) {}
        virtual void onImGuiRender() {}
        virtual void onEvent(Event& event) {}

        [[nodiscard]] const std::string& getName() const {
            return m_debugName;
        }
    };
}

