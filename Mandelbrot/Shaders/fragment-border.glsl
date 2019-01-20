#version 400

in vec2 TexCoord;

out vec4 fragColor;

uniform vec2 screen;
uniform vec4 cursor;
uniform int border;
uniform sampler2D tex;

layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;

bool isAtVerticalBorder(vec2 xy) {
    return (xy.x >= cursor.x && xy.x <= cursor.x + border)
            || (xy.x >= cursor.z - border && xy.x <= cursor.z);
}

bool isAtHorizontalBorder(vec2 xy) {
    return (xy.y >= cursor.y && xy.y <= cursor.y + border)
            || (xy.y >= cursor.w - border && xy.y <= cursor.w);
}

bool isBetweenVerticalBorder(vec2 xy) {
    return (xy.x >= cursor.x && xy.x <= cursor.z);
}

bool isBetweenHorizontalBorder(vec2 xy) {
    return (xy.y >= cursor.y && xy.y <= cursor.w);
}

void main () {
    vec2 xy = gl_FragCoord.xy;

    // if we have identical mouse positions, dont draw anything
    if (cursor.xy == cursor.zw) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    // else draw a rectangle between mouse_start and mouse_pos
    if ((isAtVerticalBorder(xy) && isBetweenHorizontalBorder(xy))
      || (isAtHorizontalBorder(xy) && isBetweenVerticalBorder(xy)))
      {
        fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    } else {
        // else we just draw the texture
//        fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
        fragColor = texture(tex, TexCoord);
    }
}