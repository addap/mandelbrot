//
// Created by adrian on 23.12.18.
//

#include "mandelbrot.h"

double square (double a) {
    return a * a;
}

void mandelbrot_rect(properties_t *properties, unsigned char pixels[SCREEN_HEIGHT][SCREEN_WIDTH][3]) {

    double delta_x = properties->width / SCREEN_WIDTH;
    double delta_y = properties->height / SCREEN_HEIGHT;

    print_view(properties);
    mpc_t val;
    mpc_init2(val, properties->precision);

    unsigned char max_val = 255;
    unsigned int max_iter = 500;
    unsigned int iter = 0;

    mpc_t a, border;
    mpc_init2(a, properties->precision);
    mpc_init2(border, 12);
    mpc_set_ui(border, 2, properties->rounding_mode);

//    mpc_init2(b, properties->precision);
//    mpc_init2(a_sqr, properties->precision);
//    mpc_init2(b_sqr, properties->precision);
//    mpc_init2(a_tmp, properties->precision);

    for (unsigned int y = 0; y < SCREEN_HEIGHT; y++) {
        mpc_set(val_x, properties->origin_x, properties->rounding_mode);

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

            mpc_set_ui(a, 0, properties->rounding_mode);
//            mpc_set_ui(b, 0, properties->rounding_mode);
//            mpc_set_ui(a_sqr, 0, properties->rounding_mode);
//            mpc_set_ui(b_sqr, 0, properties->rounding_mode);
//            mpc_set_ui(a_tmp, 0, properties->rounding_mode);

            iter = 0;

            while (mpc_cmp(a, border) <= 0 && iter < max_iter) {
                mpc_sqr(a, a, properties->rounding_mode);


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