#pragma once

namespace Bottleneck {
    class Timestep {
        float m_time;

    public:
        explicit Timestep(const float time)
            : m_time(time) {
        }

        operator float() const { return m_time; }

        [[nodiscard]] float getSeconds() const { return m_time; }
        [[nodiscard]] float getMilliseconds() const { return m_time * 1000.0f; }
    };
}
