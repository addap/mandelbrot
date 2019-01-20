//
// Created by adrian on 23.12.18.
//

#include "../Headers/mandelbrot.h"

double square (double a) {
    return a * a;
}

void mandelbrot_rect(properties_t *properties, unsigned char pixels[SCREEN_HEIGHT][SCREEN_WIDTH][3]) {

    double delta_x = properties->width / SCREEN_WIDTH;
    double delta_y = properties->height / SCREEN_HEIGHT;

    print_view(properties);
    double val_x = properties->origin_x;
    double val_y = properties->origin_y;

    unsigned char max_val = 255;
    unsigned int max_iter = 500;
    unsigned int iter = 0;

    double a = 0.0f;
    double b = 0.0f;
    double a_sqr = 0.0f;
    double b_sqr = 0.0f;
    double tmp_a = 0.0f;

    for (unsigned int y = 0; y < SCREEN_HEIGHT; y++) {
        val_x = properties->origin_x;
        for (unsigned int x = 0; x < SCREEN_WIDTH; x++) {

            //DOES NOT WORK ATM
//            //perform the cardioid check
//            double q = square(val_x - 0.25) + square(val_y);
//            double p = q * (q + (val_x - 0.25));
//            // and the period-2 bulb check
//            q = square(val_x + 1) + square(val_y);
//
//            if (p <= 0.25 * square(val_y) || q <= 0.0625) {
//                pixels[y][x][0] = 0;
//                pixels[y][x][1] = 0;
//                pixels[y][x][2] = 0;
//                return;
//            }

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