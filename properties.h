//
// Created by adrian on 23.12.18.
//

#ifndef MANDELBROT_PROPERTIES_H
#define MANDELBROT_PROPERTIES_H

#include <stdio.h>
#include "mpfr.h"

typedef struct {
    mpfr_t width;
    mpfr_t height;
    double aspect;

    mpfr_t origin_x;
    mpfr_t origin_y;

    double selection_ul_x;
    double selection_ul_y;
    double selection_lr_x;
    double selection_lr_y;

    double selection_start_x;
    double selection_start_y;

    int submit_selection;

    mpfr_prec_t precision;
    mpfr_rnd_t rounding_mode;
} properties_t;

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

void init_properties(properties_t *p);
void print_view(properties_t *p);
void print_rect(properties_t *p);

#endif //MANDELBROT_PROPERTIES_H
