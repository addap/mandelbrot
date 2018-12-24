//
// Created by adrian on 23.12.18.
//

#include "mandelbrot.h"

void mandelbrot_rect(double origin_x, double origin_y, double width, double height, unsigned char *pixels) {
    for (unsigned int y = 0; y < SCREEN_WIDTH; y++) {
        for (unsigned int x = 0; x < SCREEN_HEIGHT; x++) {
            pixels[y*SCREEN_WIDTH+x+0] = 125;
            pixels[y*SCREEN_WIDTH+x+1] = 125;
            pixels[y*SCREEN_WIDTH+x+2] = 125;
        }
    }
}