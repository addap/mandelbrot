
#include <netpbm/pam.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int debug = 0;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;
} App;

App app;


const unsigned int max_iter = 100;
const unsigned int max_color_value = 255;
unsigned int* colors;

/* returns the number of iterations until c = x + iy diverges, max_iter if it does not diverge
 */
unsigned int mandelbrot_test(double x, double y, unsigned int max_iter) {
    //todo check if pixel is in one of the large circles. Only speeds up initial calculations
    double a = 0;
    double b = 0;
    double new_a = 0;
    double new_b = 0;

    if (debug) {
        printf("Called with (%f, %f)\n", x, y);
    }

    unsigned int iterations = 0;
    while (new_a * new_a + new_b * new_b < 4 && iterations < max_iter) {
        new_a = a * a - b * b + x;
        new_b = 2 * (a * b) + y;
        iterations++;

        if (a == new_a && b == new_b) {
            break;
        }

        a = new_a;
        b = new_b;
    }

    if (debug) {
        if (iterations < max_iter) {
            printf("Took %d iterations", iterations);
        } else {
            printf("Does not diverge");
        }
    }

    return iterations;
}


void mandelbrot_calculate (int* pixels, double zoom) {

    const unsigned int width_px = SCREEN_WIDTH;
    const unsigned int height_px = SCREEN_HEIGHT;

    const double center_x = -0.7463;
    const double center_y = 0.1102;
    // width div height
    double aspect = (double)width_px / height_px;
    // modifies with and height
//    double zoom = 0.5;

    double height = (double) 1 / zoom;
    double width = aspect * height;

    double origin_x = center_x - (0.5 * width);
    double origin_y = center_y + (0.5 * height);

//    struct pam outpam;
//    outpam.format = PPM_FORMAT;
//    outpam.width = width_px;
//    outpam.height = height_px;
//    outpam.depth = 3;
//    outpam.maxval = max_color_value;
//    outpam.bytes_per_sample = 3;

//    FILE *f = fopen("./testbild.ppm", "w");
//    outpam.file = f;

//    pnm_writepaminit(&outpam);

//    tuple *tuplerow = pnm_allocpamrow(&outpam);

    double y = origin_y;
    double x_delta = (double) aspect / (width_px * zoom);
    double y_delta = (double) 1 / (height_px * zoom);

    if (debug) {
        printf("Center at (%f, %f), origin at (%f, %f), at a zoom of (%f) and height/width/aspect of (%f, %f, %f)",
               center_x, center_y, origin_x, origin_y, zoom, height, width, aspect);
    }

    for (unsigned int row = 0; row < height_px; ++row) {
        double x = origin_x;

        for (unsigned int column = 0; column < width_px; ++column) {
            unsigned long iterations = mandelbrot_test(x, y, max_iter);
            pixels[row * SCREEN_WIDTH + column] = colors[iterations];
            x += x_delta;
        }
        y -= y_delta;
//        pnm_writepamrow(&outpam, tuplerow);
    }
//    pnm_freepamrow(tuplerow);
}

void init_SDL() {

    Uint32 rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, windowFlags);

    if (!app.window) {
        printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    app.texture = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (!app.texture) {
        printf("Failed to create texture!");
        exit(1);
    }

}

void cleanup(void) {
    free(colors);

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_DestroyTexture(app.texture);
    SDL_Quit();
}

void prepare_scene(double zoom) {
    void **pixels = malloc(sizeof(void*));
    int *pitch = malloc(sizeof(int));
//    if (app.texture) {
//        printf("tex");
//        Uint32* fmt = malloc(sizeof(Uint32));
//        int* access = malloc(sizeof(int));
//        int* w = malloc(sizeof(int));
//        int* h = malloc(sizeof(int));
//        SDL_QueryTexture(app.texture, fmt, access, w, h);
//        printf("gh");
//    }
    SDL_LockTexture(app.texture, NULL, pixels, pitch);
    mandelbrot_calculate(*pixels, zoom);
//    for (int y = 0; y < SCREEN_HEIGHT; y++) {
//        for (int x = 0; x < SCREEN_WIDTH; x++) {
//            ((int*)*pixels)[y * SCREEN_WIDTH + x] = 0xffffffff;
//        }
//    }
//    printf("Pitch of texture is %d\n", *pitch);
//    printf("Pixel at position 240, 3 has value %d\n", ((int*)(*pixels))[2*SCREEN_WIDTH+240]);
    SDL_UnlockTexture(app.texture);
    free(pixels);
    free(pitch);

//    SDL_SetRenderDrawColor(app.renderer, 34, 155, 200, 70);

    SDL_RenderClear(app.renderer);
    SDL_RenderCopy(app.renderer, app.texture, NULL, NULL);

}

void present_scene() {
    SDL_RenderPresent(app.renderer);
}

void do_input(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            default:
                break;
        }
    }
}

int main(int argc, char **argv) {
    colors = malloc((max_iter+1) * sizeof(unsigned int));
    for (int i = 0; i < max_iter; i++) {
//        colors[i] = malloc(3 * sizeof(int));
        unsigned int r = max_color_value;
        unsigned int g = max_color_value - ((i * 100) % max_color_value);
        unsigned int b = (i * 100) % max_color_value;
        unsigned int a = 0;
        colors[i] = (r << 24) + (g << 16) + (b << 8) + (a << 0);
    }
    colors[max_iter] = 0;

    memset(&app, 0, sizeof(App));
    init_SDL();

    atexit(cleanup);

    double zoom = 0.5;
    while (1) {
        prepare_scene(zoom);
        do_input();
        present_scene();

        zoom *= 2;
        SDL_Delay(100);
    }
}

