//
// Created by adrian on 21.12.18.
//

#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include <math.h>
#include "window.h"
#include "shader.h"

#endif //MANDELBROT_MANDELBROT_H

typedef struct {
    // How much the zoom_time increases/decreases per second
    double zoom_scale;

    // the pixel coordinates of the center
    double x_pos;
    double y_pos;
} Properties;