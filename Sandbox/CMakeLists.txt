add_executable(Sandbox src/Sandbox.cpp)

include(src/Layers/Layers.cmake)

target_link_libraries(Sandbox PRIVATE Fall)

target_include_directories(Sandbox PRIVATE 
    ${CMAKE_SOURCE_DIR}/Fall/src
)