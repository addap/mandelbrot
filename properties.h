//
// Created by adrian on 23.12.18.
//

#ifndef MANDELBROT_PROPERTIES_H
#define MANDELBROT_PROPERTIES_H

#include <stdio.h>
#include <mpc.h>

typedef struct {
    mpc_t width;
    mpc_t height;
    float aspect;

    mpc_t origin_x;
    mpc_t origin_y;

    float selection_ul_x;
    float selection_ul_y;
    float selection_lr_x;
    float selection_lr_y;

    float selection_start_x;
    float selection_start_y;

    int submit_selection;

    mpfr_prec_t precision;
    mpc_rnd_t rounding_mode;
} properties_t;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void print_view(properties_t *p);
void print_rect(properties_t *p);

#endif //MANDELBROT_PROPERTIES_H
