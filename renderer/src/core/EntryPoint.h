#pragma once
#include "Log.h"

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
    Bottleneck::Application* app = Bottleneck::CreateApplication();
    app->run();
    delete app;
}
#endif
