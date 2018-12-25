//
// Created by adrian on 23.12.18.
//

#ifndef MANDELBROT_PROPERTIES_H
#define MANDELBROT_PROPERTIES_H

#include <stdio.h>
//#include <mpc.h>

typedef struct {
    double width;
    double height;
    float aspect;

    double origin_x;
    double origin_y;

    float selection_ul_x;
    float selection_ul_y;
    float selection_lr_x;
    float selection_lr_y;

    float selection_start_x;
    float selection_start_y;

    int submit_selection;

//    mpfr_prec_t precision;
//    mpc_rnd_t rounding_mode;
} properties_t;

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

void init_properties(properties_t *p);
void print_view(properties_t *p);
void print_rect(properties_t *p);

#endif //MANDELBROT_PROPERTIES_H
