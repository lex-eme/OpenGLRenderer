#pragma once

#ifdef BN_PLATFORM_WINDOWS
#ifdef BN_BUILD_DLL
#define BOTTLENECK_API __declspec(dllexport)
#error Testing the BUILD_DLL
#else
#define BOTTLENECK_API __declspec(dllimport)
#endif
#elif BN_PLATFORM_APPLE
// nothing yet?
#define BOTTLENECK_API
#else
#error Only support Windows and Apple
#endif


#define BN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }