add_library(FallEngine STATIC)

include(cmake/FallEngineConfig.cmake)

add_subdirectory(vendor/sdl)
add_subdirectory(vendor/spdlog)

include(cmake/Core.cmake)
include(cmake/Memory.cmake)
include(cmake/DebugTools.cmake)
include(cmake/Sdl.cmake)
include(cmake/Renderer.cmake)
include(cmake/Utils.cmake)

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