//
// Created by adrian on 23.12.18.
//

#include "../Headers/input.h"

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
/// \param window The current GLFWwindow instance
/// \param key_code The glfw code of the key to be checked
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

    // get new cursor position
    glfwGetCursorPos(window, &x_pos_new, &y_pos_new);
    float pos_x = (float)x_pos_new;
    float pos_y = (float)y_pos_new;
    aspect_case_t aspect_case;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        if (!pressed_states[MOUSE_STATE]) {
            pressed_states[MOUSE_STATE] = 1;

            properties->selection_start_x = pos_x;
            properties->selection_start_y = pos_y;
        }

        // could do that in the case analysis
        properties->selection_ul_x = fminf(properties->selection_start_x, pos_x);
        properties->selection_ul_y = fminf(properties->selection_start_y, pos_y);
        properties->selection_lr_x = fmaxf(properties->selection_start_x, pos_x);
        properties->selection_lr_y = fmaxf(properties->selection_start_y, pos_y);

        if (properties->selection_start_x <= pos_x && properties->selection_start_y <= pos_y) {
            // (lr, lr)
            aspect_case.w = 1;
            aspect_case.h = 1;
        } else if (properties->selection_start_x <= pos_x && properties->selection_start_y > pos_y) {
            // (lr, ul)
            aspect_case.w = 1;
            aspect_case.h = 0;
        } else if (properties->selection_start_x > pos_x && properties->selection_start_y > pos_y) {
            // (ul, ul)
            aspect_case.w = 0;
            aspect_case.h = 0;
        } else {
            // (ul, lr)
            aspect_case.w = 0;
            aspect_case.h = 1;
        }

        // compute the aspect we have and compare against the aspect we need. Based on that, move either the ul or lr corner
        float delta_x = properties->selection_lr_x - properties->selection_ul_x;
        float delta_y = properties->selection_lr_y - properties->selection_ul_y;
        float aspect_current = delta_x / delta_y;

        if (aspect_current < properties->aspect) {
            // increase width
            float delta_x_new = properties->aspect * delta_y;
            if (aspect_case.w) {
                // move lr corner to the right
                properties->selection_lr_x = properties->selection_ul_x + delta_x_new;
            } else {
                // move ul corner to the left
                properties->selection_ul_x = properties->selection_lr_x - delta_x_new;
            }
        } else if (aspect_current > properties->aspect) {
            //increase height
            float delta_y_new = delta_x / properties->aspect;
            if (aspect_case.h) {
                // move lr corner down
                properties->selection_lr_y = properties->selection_ul_y + delta_y_new;
            } else {
                // move ul corner up
                properties->selection_ul_y = properties->selection_lr_y - delta_y_new;
            }
        }
    } else {
        if (pressed_states[MOUSE_STATE]) {
            pressed_states[MOUSE_STATE] = 0;
            properties->submit_selection = 1;

            print_rect(properties);
            print_view(properties);
            properties->origin_x = properties->origin_x + (properties->selection_ul_x / SCREEN_WIDTH) * properties->width;
            properties->origin_y = properties->origin_y - (properties->selection_ul_y / SCREEN_HEIGHT) * properties->height;

            properties->width = ((properties->selection_lr_x - properties->selection_ul_x) / SCREEN_WIDTH) * properties->width;
            properties->height = ((properties->selection_lr_y - properties->selection_ul_y) / SCREEN_HEIGHT) * properties->height;
            print_view(properties);
        } else {
            properties->selection_ul_x = 0.0f;
            properties->selection_ul_y = 0.0f;
            properties->selection_lr_x = 0.0f;
            properties->selection_lr_y = 0.0f;
            properties->selection_start_x = 0.0f;
            properties->selection_start_y = 0.0f;
        }
    }
}