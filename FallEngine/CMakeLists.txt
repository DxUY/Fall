cmake_minimum_required(VERSION 3.16)
project(FallEngine)

# Gather all .cpp and .h files recursively
file(GLOB_RECURSE FALLENGINE_SRC CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*.h"
)

# Add the shared library
add_library(FallEngine SHARED 
    ${FALLENGINE_SRC} 
    "src/FallEngine/Events/KeyEvent.h" 
    "src/FallEngine/Events/EventBus.h" 
    "src/FallEngine/Window.h" 
    "src/Platform/Windows/WindowsWindow.h" 
    "src/Platform/Windows/WindowsWindow.cpp"
 "src/FallEngine/Layer.h" "src/FallEngine/Layer.cpp" "src/FallEngine/LayerStack.cpp" "src/FallEngine/LayerStack.h")

# Define platform and build type
target_compile_definitions(FallEngine PRIVATE 
    F_PLATFORM_WINDOWS 
    F_BUILD_DLL
)

# Include directories
target_include_directories(FallEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Precompiled header setup (requires CMake 3.16+)
target_precompile_headers(FallEngine PRIVATE 
    "src/FallEnginePCH.h"
)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(FallEngine PUBLIC spdlog::spdlog)

find_package(SDL3 REQUIRED CONFIG)
target_link_libraries(FallEngine PRIVATE SDL3::SDL3)

find_package(OpenGL REQUIRED)
target_link_libraries(FallEngine PRIVATE OpenGL::GL)

# Optional: Copy DLL after build
add_custom_command(TARGET FallEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:FallEngine>
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
    COMMENT "Copying FallEngine DLL to output directory"
)