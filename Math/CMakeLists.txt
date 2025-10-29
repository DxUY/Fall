file(GLOB_RECURSE EDITOR_SRC CONFIGURE_DEPENDS
    "src/*.h"
)

add_library(Math INTERFACE)

target_include_directories(Math INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
