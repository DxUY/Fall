file(GLOB_RECURSE EDITOR_SRC CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.h"
)

add_executable(Editor ${EDITOR_SRC})

target_link_libraries(Editor PRIVATE FallEngine)

target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/FallEngine/src)