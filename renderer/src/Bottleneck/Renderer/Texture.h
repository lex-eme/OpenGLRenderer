#pragma once

#include <string>
#include "Bottleneck/Core/Core.h"

namespace Bottleneck {
    class Texture {
    public:
        virtual ~Texture() = default;

        [[nodiscard]] virtual uint32_t getWidth() const = 0;
        [[nodiscard]] virtual uint32_t getHeight() const = 0;

        virtual void setData(void* data, uint32_t size) = 0;

        virtual void bind(uint32_t slot) const = 0;
    };

    class Texture2D : public Texture {
    public:
        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        static Ref<Texture2D> Create(const std::string& path);
    };
}
