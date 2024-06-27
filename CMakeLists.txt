cmake_minimum_required(VERSION 3.28)
project(raycaster C)

set(CMAKE_C_STANDARD 11)

set(FREEGLUT_DIR "C:/MinGW/freeglut")
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${FREEGLUT_DIR}/include)

link_directories(${FREEGLUT_DIR}/lib)
set(FREEGLUT_LIBRARIES freeglut)

add_executable(raycaster main.c)

target_link_libraries(raycaster
        ${OPENGL_gl_LIBRARY}
        ${OPENGL_glu_LIBRARY}
        ${FREEGLUT_LIBRARIES}
)