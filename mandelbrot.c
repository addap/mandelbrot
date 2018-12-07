
#include <netpbm/pam.h>
#include <stdio.h>
#include <stdlib.h>

int debug = 0;

/* returns the number of iterations until c = x + iy diverges, max_iter if it does not diverge
 */
unsigned int mandelbrot_test(double x, double y, unsigned int max_iter) {
	//todo check if pixel is in one of the large circles. Only speeds up initial calculations
	double a = 0;
	double b = 0;
	double new_a = 0;
	double new_b = 0;
	int result = -1;

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

int main(int argc, char **argv) {
 
	const unsigned int max_iter = 100;
	const unsigned int width_px = 2000;
	const unsigned int height_px = 1000;
	const unsigned int max_color_value = 255;

	const double center_x = -0.7463;
	const double center_y = 0.1102;
	// width div height
	double aspect = width_px / height_px;
	// modifies with and height
	double zoom = 20;

	if (argc == 2) {
		zoom = atof(argv[1]);
	}

	double height = (double)1 / zoom;
	double width = aspect * height;

	double origin_x = center_x - (0.5 * width);
	double origin_y = center_y + (0.5 * height);

	int colors[max_iter+1][3];
	for (int i = 0; i < max_iter; i++) {
		colors[i][0] = max_color_value;
		colors[i][1] = max_color_value - ((i * 100) % max_color_value);
		colors[i][2] = (i * 100) % max_color_value;
	}
	colors[max_iter][0] = colors[max_iter][1] = colors[max_iter][2] = 0;
	
	struct pam outpam;
	outpam.format = PPM_FORMAT;
	outpam.width = width_px;
	outpam.height = height_px;
	outpam.depth = 3;
	outpam.maxval = max_color_value;
	outpam.bytes_per_sample = 3;

	FILE *f = fopen("./testbild.ppm", "w");
	outpam.file = f;

	pnm_writepaminit(&outpam);

	tuple *tuplerow = pnm_allocpamrow(&outpam);

	double y = origin_y;
	double x_delta = (double)aspect / (width_px * zoom);
	double y_delta = (double)1 / (height_px * zoom);

	if (debug) {
		printf("Center at (%f, %f), origin at (%f, %f), at a zoom of (%f) and height/width/aspect of (%f, %f, %f)",
		       center_x, center_y, origin_x, origin_y, zoom, height, width, aspect);
	}

	for (unsigned int row = 0; row < height_px; ++row) {
		double x = origin_x;
		
		for (unsigned int column = 0; column < width_px; ++column) {
			unsigned long iterations = mandelbrot_test(x, y, max_iter);

			if (iterations == max_iter) {
				// (x, y) is in the set
			    tuplerow[column][0] = tuplerow[column][1] = tuplerow[column][2] = 0;
			} else {
				// else we compute some color for it
				tuplerow[column][0] = colors[iterations][0];
				tuplerow[column][1] = colors[iterations][1];
				tuplerow[column][2] = colors[iterations][2];
			}
			x += x_delta;
		}
		y -= y_delta;
		pnm_writepamrow(&outpam, tuplerow);
	}
	pnm_freepamrow(tuplerow);
}
