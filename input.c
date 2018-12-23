//
// Created by adrian on 23.12.18.
//

#include "input.h"

int pressed_states[4];
int UP_STATE = 0;
int DOWN_STATE = 1;
int E_STATE = 2;
int MOUSE_STATE = 3;

double x_pos_new, x_pos_scaled;
double y_pos_new, y_pos_scaled;

int code_to_index(int code) {
    switch (code) {
        case GLFW_KEY_UP:
            return 0;
        case GLFW_KEY_DOWN:
            return 1;
        case GLFW_KEY_E :
            return 2;
        default:
            fprintf(stderr, "Unknown code to retrieve index from: (%d)", code);
            exit(1);
    }
}

/// Checks if the key specified by that key_code was just pressed when this function was called.
/// If called again while the key was not registered as not pressed, returns false
/// \param window
/// \param key_code
/// \return true if the key has just been pressed and not held
int key_pressed(GLFWwindow *window, int key_code) {
    int index = code_to_index(key_code);
    if (glfwGetKey(window, key_code) == GLFW_PRESS) {
        if (!pressed_states[index]) {
            pressed_states[index] = 1;
            return 1;
        }
    } else {
        pressed_states[index] = 0;
    }
    return 0;
}

typedef struct {
    int w;
    int h;
} aspect_case_t;

void process_input(GLFWwindow *window, properties_t *properties) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    if (key_pressed(window, GLFW_KEY_UP)) {
        if (properties->zoom_scale < -ZOOM_THRESHOLD) {
            properties->zoom_scale /= 2.0f;
        } else if (properties->zoom_scale >= ZOOM_THRESHOLD) {
            properties->zoom_scale *= 2.0f;
        } else {
            properties->zoom_scale = ZOOM_THRESHOLD;
        }
    }

    if (key_pressed(window, GLFW_KEY_DOWN)) {
        if (properties->zoom_scale <= -ZOOM_THRESHOLD) {
            properties->zoom_scale *= 2.0f;
        } else if (properties->zoom_scale > ZOOM_THRESHOLD) {
            properties->zoom_scale /= 2.0f;
        } else {
            properties->zoom_scale = -ZOOM_THRESHOLD;
        }
    }

    // get new cursor position
    glfwGetCursorPos(window, &x_pos_new, &y_pos_new);
        if (key_pressed(window, GLFW_KEY_E)
        && x_pos_new >= 0.0f && x_pos_new <= SCREEN_WIDTH
        && y_pos_new >= 0.0f && y_pos_new <= SCREEN_HEIGHT)
    {
        properties->pos_x = x_pos_new;
        properties->pos_y = y_pos_new;

        // scale to screen coordinates
        x_pos_scaled = properties->pos_x / SCREEN_WIDTH;
        y_pos_scaled = properties->pos_y / SCREEN_HEIGHT;

        // make relative to center
        x_pos_scaled = x_pos_scaled - 0.5f;
        y_pos_scaled = y_pos_scaled - 0.5f;

        // calculate next center
        properties->center_x = x_pos_scaled * properties->width + properties->center_x;
        properties->center_y = y_pos_scaled * properties->height + properties->center_y;

        properties->moved_center = 1;
//            printf("Cursor is at %lf, %lf\n", x_pos_new, y_pos_new);
//            printf("WINDOWCursor is at %lf, %lf\n", pos_x, pos_y);
    } else {
        properties->moved_center = 0;
    }
}
