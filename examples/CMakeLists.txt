function(glsandbox_add_example target_name)
    add_executable(${target_name} ${ARGN})
    target_link_libraries(${target_name} PRIVATE glsandbox::glsandbox)
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/learnopengl)
    target_compile_definitions(${target_name}
        PRIVATE
            GLSANDBOX_EXAMPLE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    )

    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    set_target_properties(${target_name} PROPERTIES FOLDER examples)
endfunction()

add_subdirectory(01_textured_box_minimal)
add_subdirectory(02_textured_box_template)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT 01_textured_box_minimal)
