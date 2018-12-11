cmake_minimum_required(VERSION 3.13)
project(mandelbrot C)

set(CMAKE_C_STANDARD 11)

add_executable(mandelbrot
        mandelbrot.c)

target_link_libraries(mandelbrot
        netpbm
        SDL2)
