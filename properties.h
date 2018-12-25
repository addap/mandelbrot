//
// Created by adrian on 23.12.18.
//

#ifndef MANDELBROT_PROPERTIES_H
#define MANDELBROT_PROPERTIES_H

typedef struct {
    // How much the zoom_time increases/decreases per second
    double zoom_scale;

    // the pixel coordinates of the center
    double pos_x;
    double pos_y;

    double width;
    double height;
    float aspect;

    double center_x;
    double center_y;

    int moved_center;

    float selection_ul_x;
    float selection_ul_y;
    float selection_lr_x;
    float selection_lr_y;

    float selection_start_x;
    float selection_start_y;
} properties_t;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#endif //MANDELBROT_PROPERTIES_H
