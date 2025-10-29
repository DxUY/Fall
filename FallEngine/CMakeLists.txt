cmake_minimum_required(VERSION 3.16)
project(FallEngine)

file(GLOB_RECURSE FALLENGINE_SRC CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*.h"
)

add_library(FallEngine STATIC ${FALLENGINE_SRC})

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/FallEnginePCH.h")
    target_precompile_headers(FallEngine PRIVATE "src/FallEnginePCH.h")
endif()

target_include_directories(FallEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FallEngine
)

set(SDL_TEST OFF)
set(SDL_TESTS OFF)
set(SDL_EXAMPLES OFF)
set(SDL_SHARED OFF)  
set(SDL_STATIC ON)   
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL3" "${CMAKE_BINARY_DIR}/vendor_builds/SDL3")
target_link_libraries(FallEngine PUBLIC SDL3::SDL3)

set(SPDLOG_BUILD_EXAMPLE OFF)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_BENCH OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog" "${CMAKE_BINARY_DIR}/vendor_builds/spdlog")
target_link_libraries(FallEngine PUBLIC spdlog::spdlog)

target_include_directories(FallEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui")
target_include_directories(FallEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vulkan/Vulkan-Headers")
target_include_directories(FallEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vulkan/Vulkan-Loader")

target_link_libraries(FallEngine PUBLIC Math)