cmake_minimum_required(VERSION 3.16)

project(ChinaMaster)

add_subdirectory(base)

aux_source_directory(. ALL_SRCS)

aux_source_directory(./imgui IMGUI_SRCS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)

# add OpenCV/OpenGL/GLEW/glfw3
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OpenGL_INCLUDE_DIR})

# set path of glfw3
set(glfw3_LIBS /lib/x86_64-linux-gnu/libdl.so)
set(gl_LIBS /usr/lib/x86_64-linux-gnu/libglfw.so)

add_executable(${PROJECT_NAME} ${PROJECT_BINARY_DIR} ${ALL_SRCS} ${IMGUI_SRCS})

target_link_libraries(${PROJECT_NAME}
    base
    ${OPENGL_LIBRARIES}
    ${glfw3_LIBS}
    ${gl_LIBS}
)
