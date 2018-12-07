CFLAGS=--std=c99 -l netpbm -Werror -Wpedantic -Ofast

mandelbrot: mandelbrot.c
	cc $(CFLAGS) -o build/mandelbrot mandelbrot.c
