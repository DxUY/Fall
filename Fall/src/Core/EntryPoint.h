#pragma once

#include "Base.h"
#include "Application.h"

#ifdef FALL_PLATFORM_WINDOWS

extern Fall::Application* Fall::CreateApplication();

int main(int argc, char** argv) {
    Fall::Log::Init();

    auto app = Fall::CreateApplication();
    app->Run();
    delete app;
}

#endif