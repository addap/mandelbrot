//
// Created by adrian on 23.12.18.
//

#ifndef MANDELBROT_INPUT_H
#define MANDELBROT_INPUT_H

#include "properties.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define ZOOM_THRESHOLD 0.0625f

void process_input_cpu(GLFWwindow *window, properties_t *properties);
void process_input_gpu(GLFWwindow *window, properties_t *properties);

#endif //MANDELBROT_INPUT_H
