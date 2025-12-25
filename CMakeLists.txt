cmake_minimum_required(VERSION 3.16)
project(FallEngineProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. THE DIRECTORY FIX ---
# Forces VS to put Editor.exe in D:/dev/out/build/x64-Debug/bin/Editor.exe 
# instead of hiding it in a 'Release' or 'Debug' subfolder.
set(OUTPUT_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BIN_DIR}")

foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${OUTPUT_BIN_DIR}")
endforeach()

# --- 2. ADD SUBDIRECTORIES ---
add_subdirectory(FallEngine)

# --- 3. IDE POLISH ---
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)