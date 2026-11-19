#include "../pch.h"

#include "LayerStack.h"

namespace Bottleneck {
    LayerStack::~LayerStack() {
        for (Layer* layer: m_Layers) {
            layer->onDetach();
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex += 1;
    }

    void LayerStack::pushOverlay(Layer* overlay) {
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::popLayer(Layer* layer) {
        const auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex) {
            layer->onDetach();
            m_Layers.erase(it);
            m_LayerInsertIndex -= 1;
        }
    }

    void LayerStack::popOverlay(Layer* overlay) {
        const auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end()) {
            overlay->onDetach();
            m_Layers.erase(it);
        }
    }
}
