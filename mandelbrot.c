//
// Created by adrian on 23.12.18.
//

#include "mandelbrot.h"

double square (double a) {
    return a * a;
}

void mandelbrot_rect(properties_t *properties, unsigned char pixels[SCREEN_HEIGHT][SCREEN_WIDTH][3]) {

    mpfr_t delta_x, delta_y;
    mpfr_init2(delta_x, properties->precision);
    mpfr_init2(delta_y, properties->precision);

    mpfr_div_d(delta_x, properties->width, SCREEN_WIDTH, properties->rounding_mode);
    mpfr_div_d(delta_y, properties->height, SCREEN_HEIGHT, properties->rounding_mode);

    print_view(properties);
    mpc_t val;
    mpfr_t val_x;
    mpfr_t val_y;
    mpc_init2(val, properties->precision);
    mpfr_init2(val_x, properties->precision);
    mpfr_init2(val_y, properties->precision);

    unsigned char max_val = 255;
    unsigned int max_iter = 500;
    unsigned int iter = 0;

    mpc_t a, border;
    mpc_init2(a, properties->precision);
    mpc_init2(border, properties->precision);
    mpc_set_ui(border, 2, properties->rounding_mode);

    mpfr_set(val_y, properties->origin_y, properties->rounding_mode);
    int rnd = MPC_RNDNN;

    for (unsigned int y = 0; y < SCREEN_HEIGHT; y++) {
        mpfr_set(val_x, properties->origin_x, properties->rounding_mode);

        for (unsigned int x = 0; x < SCREEN_WIDTH; x++) {
            mpc_set_fr_fr(val, val_x, val_y, rnd);
//            if (y == 0) {
//                char * val_str = mpc_get_str(10, 0, val, properties->rounding_mode);
//                printf("Checking pixel (%u, %u) at %s\n", x, y, val_str);
//                mpc_free_str(val_str);
//            }
            mpc_set_ui(a, 0, rnd);
            iter = 0;
            int cmp;
            int re = 0;
            int im = 0;

            while (re <= 0 && im <= 0 && iter < max_iter) {
                mpc_sqr(a, a, rnd);
                mpc_add(a, a, val, rnd);

                cmp = mpc_cmp(a, border);
                re = MPC_INEX_RE(cmp);
                im = MPC_INEX_IM(cmp);
                iter++;
            }
//            printf("%u\n", iter);

            double factor = sqrt((double)iter / max_iter);
            unsigned char c = (unsigned char)lround(factor * 255);
//            printf("%hhu\n", c);

            if (iter == max_iter) {
                pixels[y][x][0] = 0;
                pixels[y][x][1] = 0;
                pixels[y][x][2] = 0;
            } else {
                pixels[y][x][0] = c;
                pixels[y][x][1] = c;
                pixels[y][x][2] = c;
            }

            mpfr_add(val_x, val_x, delta_x, properties->rounding_mode);
        }
        mpfr_sub(val_y, val_y, delta_y, properties->rounding_mode);
    }
}