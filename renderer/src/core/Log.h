#pragma once

#include <memory>

#include "spdlog/spdlog.h"

namespace Bottleneck {
    class Log {
    private:
        inline static std::shared_ptr<spdlog::logger> s_coreLogger;
        inline static std::shared_ptr<spdlog::logger> s_clientLogger;

    public:
        static void init();

        static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
        static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }
    };
}

#ifdef BN_BUILD_RELEASE
#define BN_CORE_TRACE
#define BN_CORE_INFO
#define BN_CORE_WARN
#define BN_CORE_ERROR
#define BN_TRACE
#define BN_INFO
#define BN_WARN
#define BN_ERROR
#else
// Core log macros
#define BN_CORE_TRACE(...) ::Bottleneck::Log::getCoreLogger()->trace(__VA_ARGS__)
#define BN_CORE_INFO(...) ::Bottleneck::Log::getCoreLogger()->info(__VA_ARGS__)
#define BN_CORE_WARN(...) ::Bottleneck::Log::getCoreLogger()->warn(__VA_ARGS__)
#define BN_CORE_ERROR(...) ::Bottleneck::Log::getCoreLogger()->error(__VA_ARGS__)

// Core client macros
#define BN_TRACE(...) ::Bottleneck::Log::getClientLogger()->trace(__VA_ARGS__)
#define BN_INFO(...) ::Bottleneck::Log::getClientLogger()->info(__VA_ARGS__)
#define BN_WARN(...) ::Bottleneck::Log::getClientLogger()->warn(__VA_ARGS__)
#define BN_ERROR(...) ::Bottleneck::Log::getClientLogger()->error(__VA_ARGS__)
#endif
