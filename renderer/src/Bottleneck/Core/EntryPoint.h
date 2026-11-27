#pragma once
#include "Log.h"
#include "Bottleneck/Debug/Instrumentor.h"

#ifdef BN_PLATFORM_WINDOWS
#include "Application.h"

extern Bottleneck::Application* Bottleneck::CreateApplication();

int main(int argc, char** argv) {
    Bottleneck::Log::init();
    Bottleneck::Application* app = Bottleneck::CreateApplication();
    app->run();
    delete app;
}
#endif

#ifdef BN_PLATFORM_APPLE
#include "Application.h"

extern Bottleneck::Application* Bottleneck::CreateApplication();

int main(int argc, char** argv) {
    Bottleneck::Log::init();

    BN_PROFILE_BEGIN_SESSION("Startup", "BottleneckProfile-Startup.json");
    Bottleneck::Application* app = Bottleneck::CreateApplication();
    BN_PROFILE_END_SESSION();

    BN_PROFILE_BEGIN_SESSION("Runtime", "BottleneckProfile-Runtime.json");
    app->run();
    BN_PROFILE_END_SESSION();

    BN_PROFILE_BEGIN_SESSION("Shutdown", "BottleneckProfile-Shutdown.json");
    delete app;
    BN_PROFILE_END_SESSION();
}
#endif
