cmake_minimum_required(VERSION 3.16)
project(Runner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(runner_animation
    src/main.cpp
    src/render/Sprite.cpp
    src/render/ParallaxLayer.cpp
)

target_include_directories(runner_animation PUBLIC include)

find_package(glfw3 CONFIG REQUIRED)

if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)

    target_link_libraries(runner_animation PRIVATE
        glfw
        ${OPENGL_LIBRARY}
    )
else()
    find_package(OpenGL REQUIRED)

    target_link_libraries(runner_animation PRIVATE
        glfw
        OpenGL::GL
        pthread
        dl
    )
endif()

if(APPLE)
    set(GLFW_CLIENT_API "GLFW_OPENGL_CORE_PROFILE")
elseif(UNIX AND NOT APPLE)
    set(GLFW_CLIENT_API "GLFW_OPENGL_ES_API")
endif()

target_compile_definitions(runner_animation PRIVATE ${GLFW_CLIENT_API}_DEFINED=1)
