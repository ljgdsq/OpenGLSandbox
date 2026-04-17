set(GLSANDBOX_HEADERS
    include/glsandbox/glsandbox.h
    include/glsandbox/shader.h
    include/glsandbox/texture.h
    include/glsandbox/window.h
)

set(GLSANDBOX_SOURCES
    src/stb_image_impl.cpp
    src/texture.cpp
    src/window.cpp
    ../third/glad/src/gl.c
)

add_library(glsandbox STATIC ${GLSANDBOX_HEADERS} ${GLSANDBOX_SOURCES})
add_library(glsandbox::glsandbox ALIAS glsandbox)

target_include_directories(glsandbox
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/third/glad/include
        ${PROJECT_SOURCE_DIR}/third/stb
    PRIVATE
        ${PROJECT_SOURCE_DIR}/third/glfw/include
)

target_link_libraries(glsandbox
    PUBLIC
        glfw
        glm::glm
)

target_compile_definitions(glsandbox
    PUBLIC
        GLSANDBOX_ASSET_DIR="${PROJECT_SOURCE_DIR}/assets"
)

if(MSVC)
    target_compile_options(glsandbox PRIVATE /W4 /permissive-)
else()
    target_compile_options(glsandbox PRIVATE -Wall -Wextra -Wpedantic)
endif()

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${GLSANDBOX_HEADERS} ${GLSANDBOX_SOURCES})
set_target_properties(glsandbox PROPERTIES FOLDER core)
