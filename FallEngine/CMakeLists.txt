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

    src/FallEngine/Memory/MappedFile.cpp
 
    src/FallEngine/Renderer/GraphicsContext.cpp
    src/FallEngine/Renderer/Renderer.cpp
    src/FallEngine/Renderer/ComputeShader.cpp
    src/FallEngine/Renderer/GraphicsShader.cpp
    src/FallEngine/Renderer/RenderPass.cpp
    src/FallEngine/Renderer/Texture.cpp
    
    src/FallEngine/Sdl/SdlWindow.cpp
    src/FallEngine/Sdl/SdlInput.cpp
    src/FallEngine/Sdl/SdlUtils.cpp
)

set(SDL_TEST OFF)
set(SDL_TESTS OFF)
set(SDL_EXAMPLES OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl")

set(SPDLOG_BUILD_EXAMPLE OFF)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_BENCH OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog")

add_library(FallEngine STATIC ${FALLENGINE_SRC})

target_include_directories(FallEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FallEngine
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
    
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/FallEnginePCH.h")
    target_precompile_headers(FallEngine PRIVATE "src/FallEnginePCH.h")
endif()

target_link_libraries(FallEngine
    PUBLIC
        SDL3::SDL3
        spdlog::spdlog
)