add_executable(Editor src/Editor.cpp)

include(src/Layers/Layers.cmake)

target_link_libraries(Editor PRIVATE Fall)

target_include_directories(Editor PRIVATE 
    ${CMAKE_SOURCE_DIR}/Fall/src
)