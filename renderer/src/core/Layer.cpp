#include "../pch.h"

#include "Layer.h"

namespace Bottleneck {
    Layer::Layer(const std::string& name)
        : m_debugName(name) {
    }

    Layer::~Layer() {
    }
}
