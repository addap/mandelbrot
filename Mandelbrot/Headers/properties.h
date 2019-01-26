//
// Created by adrian on 23.12.18.
//

#ifndef MANDELBROT_PROPERTIES_H
#define MANDELBROT_PROPERTIES_H

#include <stdio.h>

typedef struct {
    double width;
    double height;
    float aspect;

    double origin_x;
    double origin_y;

    double center_x;
    double center_y;

    float selection_ul_x;
    float selection_ul_y;
    float selection_lr_x;
    float selection_lr_y;

    float selection_start_x;
    float selection_start_y;

    int submit_selection;
    int moved_center;
    double zoom_scale;

} properties_t;

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

void print_view(properties_t *p);
void print_rect(properties_t *p);

#endif //MANDELBROT_PROPERTIES_H
