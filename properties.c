//
// Created by adrian on 24.12.18.
//

#include "properties.h"

void init_properties(properties_t *p) {
    p->aspect = (double)SCREEN_WIDTH / SCREEN_HEIGHT;
    p->submit_selection = 1;
    p->precision = 128;
    p->rounding_mode = MPFR_RNDN;

    mpfr_init2(p->height, p->precision);
    mpfr_init2(p->width, p->precision);
    mpfr_init2(p->origin_x, p->precision);
    mpfr_init2(p->origin_y, p->precision);

    double height = 2.0;
    double width = p->aspect * height;
    mpfr_set_d(p->width,  width, p->rounding_mode);
    mpfr_set_d(p->height, height, p->rounding_mode);

    mpfr_set_d(p->origin_x, -0.5 - width / 2, p->rounding_mode);
    mpfr_set_d(p->origin_y, height / 2, p->rounding_mode);
}

void print_view(properties_t *p) {
    char * origin_x_str, * origin_y_str, * width_str, * height_str;
    mpfr_exp_t origin_x_exp, origin_y_exp, width_exp, height_exp;
    origin_x_str = mpfr_get_str(NULL, &origin_x_exp, 10, 0, p->origin_x, p->rounding_mode);
    origin_y_str = mpfr_get_str(NULL, &origin_y_exp, 10, 0, p->origin_y, p->rounding_mode);
    width_str = mpfr_get_str(NULL, &width_exp, 10, 0, p->width, p->rounding_mode);
    height_str = mpfr_get_str(NULL, &height_exp, 10, 0, p->height, p->rounding_mode);

    printf("Origin: (%sE%ld, %sE%ld)\tMeasures: (%sE%ld, %sE%ld)\n", origin_x_str, origin_x_exp, origin_y_str, origin_y_exp, width_str, width_exp, height_str, height_exp);

    mpfr_free_str(origin_x_str);
    mpfr_free_str(origin_y_str);
    mpfr_free_str(width_str);
    mpfr_free_str(height_str);
}

void print_rect(properties_t *p) {
    printf("UL: (%lf, %lf)\tLR: (%lf, %lf)\n", p->selection_ul_x, p->selection_ul_y, p->selection_lr_x, p->selection_lr_y);
}