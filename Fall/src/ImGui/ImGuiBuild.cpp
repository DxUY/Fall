// 1. THIS MUST BE FIRST
#define IMGUI_DEFINE_MATH_OPERATORS

// 2. Your Precompiled Header
#include "FallPCH.h"

// 3. Core ImGui
#include <imgui.cpp>
#include <imgui_draw.cpp>
#include <imgui_widgets.cpp>
#include <imgui_tables.cpp>

// 4. Extensions and Backends
#include <misc/cpp/imgui_stdlib.cpp>
#include "imgui_impl_sdl3.cpp"
#include "imgui_impl_sdlgpu3.cpp"