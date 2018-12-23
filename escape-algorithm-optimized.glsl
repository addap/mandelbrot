#version 400 core

out vec4 fragColor;

uniform dvec2 center;
uniform dvec2 wh;
uniform dvec2 screen;

layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;

double square (double a) {
    return a * a;
}

void main()
{
    dvec2 xy = gl_FragCoord.xy;

    // scale to screen coordinates
    xy.x = xy.x / screen.x;
    xy.y = xy.y / screen.y;

    // transform to real numbers in the current zoom window
    dvec2 halfs = dvec2(0.5, 0.5);
    xy = wh * (xy - halfs) + center;

    //perform the cardioid check
    double q = square(xy.x - 0.25) + square(xy.y);
    double p = q * (q + (xy.x - 0.25));
    // and the period-2 bulb check
    q = square(xy.x + 1) + square(xy.y);

    if (p <= 0.25 * square(xy.y) || q <= 0.0625) {
        fragColor = vec4 (0.0, 0.0, 0.0, 1.0);
        return;
    }


    double a = 0.0;
    double b = 0.0;
    double a_tmp = 0.0;
    double iteration = 0.0;
    // DAS ANPASSEN, VERLANGSAMT ABER STARK
    double max_iter = 500.0;

    double a_sqr = a * a;
    double b_sqr = b * b;
    
    while (a_sqr + b_sqr <= 4.0 && iteration < max_iter) {
        a_tmp = a_sqr - b_sqr + xy.x;
        b = a * b;
        b += b + xy.y;
        a = a_tmp;

        a_sqr = a * a;
        b_sqr = b * b;
        iteration++;
    }
    
    if (iteration == max_iter) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = vec4(1.0 - iteration / max_iter, iteration / max_iter, 0.7, 1.0);
    }
}