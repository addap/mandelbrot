//
// Created by adrian on 24.12.18.
//

#include "../Headers/properties.h"

void print_view(properties_t *p) {
    printf("Origin: (%lf, %lf)\tMeasures: (%lf, %lf)\n", p->origin_x, p->origin_y, p->width, p->height);
}

void print_rect(properties_t *p) {
    printf("UL: (%lf, %lf)\tLR: (%lf, %lf)\n", p->selection_ul_x, p->selection_ul_y, p->selection_lr_x, p->selection_lr_y);
}