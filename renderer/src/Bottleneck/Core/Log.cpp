#include "pch.h"

#include "Log.h"


namespace Bottleneck {
    void Log::init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_coreLogger = spdlog::stdout_color_mt("BOTTLENECK");
        s_coreLogger->set_level(spdlog::level::trace);
        s_clientLogger = spdlog::stdout_color_mt("APP");
        s_clientLogger->set_level(spdlog::level::trace);
    }
}
