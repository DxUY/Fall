cmake_minimum_required(VERSION 3.20)
project(FallEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FALLENGINE_SRC
    src/FallEngine/Core/Application.cpp
    src/FallEngine/Core/FileSystem.cpp
    src/FallEngine/Core/Layer.cpp
    src/FallEngine/Core/LayerStack.cpp
    src/FallEngine/Core/Log.cpp
 
    src/FallEngine/Renderer/GraphicsContext.cpp
    src/FallEngine/Renderer/Resources/Shader.cpp

    src/Platform/Sdl/SdlWindow.cpp
)

set(SDL_TEST OFF)
set(SDL_TESTS OFF)
set(SDL_EXAMPLES OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl" "${CMAKE_BINARY_DIR}/vendor_builds/sdl")

set(SPDLOG_BUILD_EXAMPLE OFF)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_BENCH OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog" "${CMAKE_BINARY_DIR}/vendor_builds/spdlog")

add_library(FallEngine STATIC ${FALLENGINE_SRC})

target_include_directories(FallEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FallEngine
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/shadercross/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/FallEnginePCH.h")
    target_precompile_headers(FallEngine PRIVATE "src/FallEnginePCH.h")
endif()

target_link_libraries(FallEngine
    PUBLIC
        SDL3::SDL3
        spdlog
)