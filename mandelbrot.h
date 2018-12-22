//
// Created by adrian on 21.12.18.
//

#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include <math.h>
#include "window.h"
#include "shader.h"

#define ZOOM_THRESHOLD 0.0625f

typedef struct {
    // How much the zoom_time increases/decreases per second
    double zoom_scale;

    // the pixel coordinates of the center
    double pos_x;
    double pos_y;

    double width;
    double height;
    double aspect;

    double center_x;
    double center_y;

    int moved_center;
} Properties;

#endif //MANDELBROT_MANDELBROT_H
