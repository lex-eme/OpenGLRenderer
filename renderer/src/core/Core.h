#pragma once

#include <memory>

#ifdef BN_PLATFORM_WINDOWS
#elif BN_PLATFORM_APPLE
#else
#error Only support Windows and Apple
#endif

#define BN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Bottleneck {
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Ref = std::shared_ptr<T>;
}