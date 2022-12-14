cmake_minimum_required(VERSION 3.20)

project(biljard)

set(CXX_STANDARD C++17)
set(CXX_STANDARD_REQUIRED true)

add_executable(${PROJECT_NAME}
    src/Main.cpp
    src/Renderer.cpp
    src/Renderer.h
    src/Ball.cpp
    src/Ball.h
    src/Collision.cpp
    src/Collision.h
    src/Cue.cpp
    src/Cue.h
    src/Physics.cpp
    src/Physics.h

    deps/glad/src/glad.c
    deps/stb_image/stb_image.cpp
)

add_subdirectory(deps/glfw-3.3.8)
add_subdirectory(deps/glm-0.9.9.8)

target_include_directories(${PROJECT_NAME} PRIVATE
    deps/glad/include
    deps/glfw-3.3.8/include
    deps/glm-0.9.9.8/
    deps/stb_image/
)

target_link_libraries(${PROJECT_NAME}
    glfw
    glm
)