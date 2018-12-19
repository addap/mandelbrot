#version 400 core

out vec4 fragColor;

uniform vec2 center;
uniform vec2 wh;
uniform vec2 screen;

void main()
{
    vec2 xy = gl_FragCoord.xy;

    xy.x = xy.x / screen.x;
    xy.y = xy.y / screen.y;

    //double center_x = 0.3750001200618655;
    //double center_y = -0.2166393884377127;

    //double origin_x = center_x - 0.5 * width;
    //double origin_y = center_y - 0.5 * height;

    vec2 halfs = vec2(0.5, 0.5);
    xy = wh * (xy - halfs) + center;
    //xy.x = width * xy.x + origin_x;
    //xy.y = height * xy.y + origin_y;
    
    double a = 0.0;
    double b = 0.0;
    double new_a = 0.0;
    double new_b = 0.0;
    double iteration = 0.0;
    double max_iter = 100.0;
    
    while (a * a + b * b <= 4.0 && iteration < max_iter) {
        new_a = a * a - b * b + xy.x;
        new_b = 2.0 * a * b + xy.y;
        
        iteration++;
        a = new_a;
        b = new_b;
    }
    
    if (iteration == max_iter) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = vec4(iteration / max_iter, 1.0 - iteration / max_iter, 0.7, 1.0);
    }
}