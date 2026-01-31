cmake_minimum_required(VERSION 3.16)

project(FallEngine
    VERSION 0.1.0
    LANGUAGES CXX
)

set(OUTPUT_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BIN_DIR}")

if(CMAKE_CONFIGURATION_TYPES)
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${CONFIG} CONFIG_UPPER)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${OUTPUT_BIN_DIR}")
    endforeach()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(Fall)
add_subdirectory(Editor)

set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT Editor
)