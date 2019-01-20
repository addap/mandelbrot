cmake_minimum_required(VERSION 3.13)
project(mandelbrot C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c99")

include_directories(Glitter/Headers/
                    Glitter/Vendor/glad/include/
                    Glitter/Vendor/glfw/include/)

file(GLOB VENDORS_SOURCES Glitter/Vendor/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Glitter/Headers/*.h)
file(GLOB PROJECT_SOURCES Glitter/Sources/*.c)
file(GLOB PROJECT_SHADERS Glitter/Shaders/*.glsl)

file(GLOB PROJECT_CONFIGS CMakeLists.txt
        Readme.md
        .gitattributes
        .gitignore
        .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE)
        #-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} glfw
                                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)



#add_executable(mandelbrot
#        Mandelbrot/Sources/main.c
#        glad.c Mandelbrot/Sources/window.c Mandelbrot/Headers/window.h Mandelbrot/Headers/main.h Mandelbrot/Sources/shader.c Mandelbrot/Headers/shader.h Mandelbrot/Sources/input.c Mandelbrot/Headers/input.h Mandelbrot/Headers/properties.h Mandelbrot/Sources/mandelbrot.c Mandelbrot/Headers/mandelbrot.h Mandelbrot/Sources/properties.c)
#
#target_link_libraries(mandelbrot
#        GL
#        glfw
#        X11
#        pthread
#        Xrandr
#        Xi
#        m
#        dl)
#