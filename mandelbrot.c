//
// Created by adrian on 23.12.18.
//

#include "mandelbrot.h"

void mandelbrot_rect(double origin_x, double origin_y, double width, double height, unsigned char pixels[SCREEN_HEIGHT][SCREEN_WIDTH][3]) {

    double delta_x = width / SCREEN_WIDTH;
    double delta_y = height / SCREEN_HEIGHT;

    double val_x = origin_x;
    double val_y = origin_y;

    unsigned char max_val = 255;
    unsigned int max_iter = 100;
    unsigned int iter = 0;

    double a = 0.0f;
    double b = 0.0f;
    double a_sqr = 0.0f;
    double b_sqr = 0.0f;
    double tmp_a = 0.0f;

    for (unsigned int y = 0; y < SCREEN_HEIGHT; y++) {
        val_x = origin_x;
        for (unsigned int x = 0; x < SCREEN_WIDTH; x++) {
            a = b = tmp_a = a_sqr = b_sqr = 0.0f;
            iter = 0;

            while (a_sqr + b_sqr <= 4.0 && iter < max_iter) {
                tmp_a = a_sqr - b_sqr + val_x;
                b = 2 * a * b + val_y;
                a = tmp_a;
                a_sqr = a*a;
                b_sqr = b*b;

                iter++;
            }

            double factor = sqrt((double)iter / max_iter);
            unsigned char c = (unsigned char)lround(factor * 255);

            if (iter == max_iter) {
                pixels[y][x][0] = 0;
                pixels[y][x][1] = 0;
                pixels[y][x][2] = 0;
            } else {
                pixels[y][x][0] = c;
                pixels[y][x][1] = c;
                pixels[y][x][2] = c;
            }

            val_x += delta_x;
        }
        val_y -= delta_y;
    }
}