//
// Created by adrian on 23.12.18.
//

#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include "properties.h"
#include "mpc.h"
#include <stdio.h>
#include <math.h>

void mandelbrot_rect(properties_t *properties, unsigned char pixels[SCREEN_HEIGHT][SCREEN_WIDTH][3]);

#endif //MANDELBROT_MANDELBROT_H
