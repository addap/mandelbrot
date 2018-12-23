cmake_minimum_required(VERSION 3.13)
project(mandelbrot C)

set(CMAKE_C_STANDARD 11)

add_executable(mandelbrot
        main.c
        glad.c window.c window.h main.h shader.c shader.h input.c input.h properties.h mandelbrot.c mandelbrot.h)

target_link_libraries(mandelbrot
        GL
        glfw
        X11
        pthread
        Xrandr
        Xi
        m
        dl)

target_compile_options(mandelbrot PRIVATE
        -Wall
        -Wextra)
