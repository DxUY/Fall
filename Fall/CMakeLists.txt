include(Config.cmake)

add_library(Fall STATIC)

add_subdirectory(vendor/sdl)
add_subdirectory(vendor/spdlog)

include(src/Core/Core.cmake)
include(src/Memory/Memory.cmake)
include(src/DebugTools/DebugTools.cmake)
include(src/Sdl/Sdl.cmake)
include(src/Renderer/Renderer.cmake)
include(src/Utils/Utils.cmake)

target_include_directories(Fall PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/FallPCH.h")
    target_precompile_headers(Fall PRIVATE "src/FallPCH.h")
endif()

target_link_libraries(Fall
    PUBLIC
        SDL3::SDL3
        spdlog::spdlog
)