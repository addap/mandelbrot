cmake_minimum_required(VERSION 3.13)
project(mandelbrot C)

set(CMAKE_C_STANDARD 11)

add_executable(mandelbrot
        mandelbrot.c
        glad.c)

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
