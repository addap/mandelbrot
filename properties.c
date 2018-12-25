//
// Created by adrian on 24.12.18.
//

#include "properties.h"

void init_properties(properties_t *p) {
    p->aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
    p->submit_selection = 1;
    p->precision = 128;
    p->rounding_mode = MPC_RNDNN;

    mpc_init2(p->height, p->precision);
    mpc_init2(p->width, p->precision);
    mpc_init2(p->origin_x, p->precision);
    mpc_init2(p->origin_y, p->precision);
    
    p->height = 2.0;
    p->width = p->aspect * p->height;
    p->origin_x = - 0.5 - p->width / 2;
    p->origin_y = p->height / 2;
}

void print_view(properties_t *p) {
    printf("Origin: (%lf, %lf)\tMeasures: (%lf, %lf)\n", p->origin_x, p->origin_y, p->width, p->height);
}

void print_rect(properties_t *p) {
    printf("UL: (%lf, %lf)\tLR: (%lf, %lf)\n", p->selection_ul_x, p->selection_ul_y, p->selection_lr_x, p->selection_lr_y);
}