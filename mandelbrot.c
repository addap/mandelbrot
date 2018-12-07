
#include <netpbm/pam.h>
#include <stdio.h>

/* returns the number of iterations until c = x + iy diverges, 0 if it does not diverge
 */
unsigned long mandelbrot_test(double x, double y, unsigned long max_iter) {
	double a = 0;
	double b = 0;
	double new_a = 0;
	double new_b = 0;
	/* printf("Called with (%f, %f)\n", x, y); */

	for (unsigned long i = 0; i < max_iter; i++) {
		new_a = a * a - b * b + x;
		new_b = 2 * (a * b) + y;
		

		if (new_a * new_a + new_b * new_b >= 4) {
			/* printf("Took %d iterations", i+1); */
			return i;
		}

		a = new_a;
		b = new_b;
	}

	/* printf("Does not diverge"); */
	return max_iter;
}

int main(int argc, char **argv) {
	printf("%lu\n", mandelbrot_test(-0.5, 1, 1000));


	struct pam outpam;
	outpam.format = PPM_FORMAT;
	outpam.width = 2000;
	outpam.height = 1000;
	outpam.depth = 3;
	outpam.maxval = 255;
	outpam.bytes_per_sample = 3;

	FILE *f = fopen("./testbild.ppm", "w");
	outpam.file = f;

	pnm_writepaminit(&outpam);

	tuple *tuplerow = pnm_allocpamrow(&outpam);
	/* unsigned long grand_total; */

	for (unsigned int row = 0; row < outpam.height; ++row) {
		double y = 1 - ((double)row / 500);
		for (unsigned int column = 0; column < outpam.width; ++column) {
			double x = -1.5 + ((double)column / 1000);

			unsigned long iterations = mandelbrot_test(x, y, 100);
			/* fprintf(f, "(%lu)", iterations); */

			if (iterations == 100) {
			    tuplerow[column][0] = tuplerow[column][1] = tuplerow[column][2] = 0;
			} else {
			     tuplerow[column][2] = (iterations * 100) % (outpam.maxval + 1);
			     tuplerow[column][1] = outpam.maxval - ((iterations * 100) % (outpam.maxval + 1));
			     tuplerow[column][0] = outpam.maxval;
			}

		}
		/* fprintf(f, "\n"); */
		pnm_writepamrow(&outpam, tuplerow);
	}
	pnm_freepamrow(tuplerow);
}
