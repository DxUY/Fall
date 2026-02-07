function(configure_fall_target target)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    target_compile_definitions(${target}
        PUBLIC FALL_ENGINE
        PRIVATE $<$<CONFIG:Debug>:FALL_DEBUG>
    )

    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /permissive-
            /Zc:__cplusplus
            /EHsc
            /GR
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()

    if(MSVC)
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Release>:/O2>
        )
    else()
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Release>:-O2>
        )
    endif()

    if(FALL_ENABLE_SANITIZERS AND NOT MSVC)
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address,undefined>
            $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )
        target_link_options(${target} PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address,undefined>
        )
    endif()

    if(FALL_PERF_BUILD)
        if(MSVC)
            target_compile_options(${target} PRIVATE
                /O2
                /GL
            )
            set_property(TARGET ${target}
                PROPERTY INTERPROCEDURAL_OPTIMIZATION ON
            )
        else()
            target_compile_options(${target} PRIVATE
                -O3
                -ffast-math
            )
        endif()
    endif()
endfunction()