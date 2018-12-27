//
// Created by adrian on 24.12.18.
//

#include "properties.h"

void init_properties(properties_t *p) {
    p->aspect = (double)SCREEN_WIDTH / SCREEN_HEIGHT;
    p->submit_selection = 1;
    p->precision = 128;
    p->rounding_mode = MPC_RNDNN;

    mpfr_init2(p->height, p->precision);
    mpfr_init2(p->width, p->precision);
    mpc_init2(p->origin, p->precision);

    double width = 2.0;
    double height = p->height * p->aspect;
    mpfr_set_d(p->height, width, p->rounding_mode);
    mpfr_set_d(p->width,  height, p->rounding_mode);

    mpc_set_ld_ld(p->origin, -0.5 - width / 2, height / 2);
}

void print_view(properties_t *p) {
    printf("Origin: %s\tMeasures: (%lf, %lf)\n", mpc_get_str(10, 0, p->origin, p->rounding_mode), p->width, p->height);
}

void print_rect(properties_t *p) {
    printf("UL: (%lf, %lf)\tLR: (%lf, %lf)\n", p->selection_ul_x, p->selection_ul_y, p->selection_lr_x, p->selection_lr_y);
}