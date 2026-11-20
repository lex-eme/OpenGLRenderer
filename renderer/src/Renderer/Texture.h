#pragma once

#include <string>
#include "core/Core.h"

namespace Bottleneck {
    class Texture {
    public:
        virtual ~Texture() = default;

        [[nodiscard]] virtual uint32_t getWidth() const = 0;
        [[nodiscard]] virtual uint32_t getHeight() const = 0;

        virtual void bind(uint32_t slot) const = 0;
    };

    class Texture2D : public Texture {
    public:
        static Ref<Texture2D> Create(const std::string& path);
    };
}
