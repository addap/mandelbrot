//
// Created by adrian on 21.12.18.
//

#ifndef MANDELBROT_WINDOW_H
#define MANDELBROT_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "properties.h"

GLFWwindow* createWindow();
GLuint createArrayBuffer ();

#endif //MANDELBROT_WINDOW_H
