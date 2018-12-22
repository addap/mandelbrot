#include "mandelbrot.h"

Properties properties = {
        .zoom_scale = 1.0f,
        .pos_x = 0.0f,
        .pos_y = 0.0f,
        .aspect = (double)SCREEN_WIDTH / SCREEN_HEIGHT,
        .center_x = 0.0f,
        .center_y = 0.0f,
        .moved_center = 0
};

int pressed_states[3];
int UP_STATE = 0;
int DOWN_STATE = 1;
int E_STATE = 2;

double x_pos_new, x_pos_scaled;
double y_pos_new, y_pos_scaled;

const float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
};

const unsigned int indices[] = {
        0, 1, 2,
        1, 3, 2
};

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

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    if (key_pressed(window, GLFW_KEY_UP)) {
        if (properties.zoom_scale < -ZOOM_THRESHOLD) {
            properties.zoom_scale /= 2.0f;
        } else if (properties.zoom_scale >= ZOOM_THRESHOLD) {
            properties.zoom_scale *= 2.0f;
        } else {
            properties.zoom_scale = ZOOM_THRESHOLD;
        }
    }

    if (key_pressed(window, GLFW_KEY_DOWN)) {
        if (properties.zoom_scale <= -ZOOM_THRESHOLD) {
            properties.zoom_scale *= 2.0f;
        } else if (properties.zoom_scale > ZOOM_THRESHOLD) {
            properties.zoom_scale /= 2.0f;
        } else {
            properties.zoom_scale = -ZOOM_THRESHOLD;
        }
    }

    /// CALC NEW CENTER
    // get new cursor position
    glfwGetCursorPos(window, &x_pos_new, &y_pos_new);
    if (key_pressed(window, GLFW_KEY_E)
        && x_pos_new >= 0.0f && x_pos_new <= SCREEN_WIDTH
        && y_pos_new >= 0.0f && y_pos_new <= SCREEN_HEIGHT)
    {
        properties.pos_x = x_pos_new;
        properties.pos_y = y_pos_new;

        // scale to screen coordinates
        x_pos_scaled = properties.pos_x / SCREEN_WIDTH;
        y_pos_scaled = properties.pos_y / SCREEN_HEIGHT;

        // make relative to center
        x_pos_scaled = x_pos_scaled - 0.5f;
        y_pos_scaled = y_pos_scaled - 0.5f;

        // calculate next center
        properties.center_x = x_pos_scaled * properties.width + properties.center_x;
        properties.center_y = - y_pos_scaled * properties.height + properties.center_y;

        properties.moved_center = 1;
//            printf("Cursor is at %lf, %lf\n", x_pos_new, y_pos_new);
//            printf("WINDOWCursor is at %lf, %lf\n", pos_x, pos_y);
    } else {
        properties.moved_center = 0;
    }
}

void createBufferObjects (GLuint* VAO, GLuint* VBO, GLuint* EBO) {


    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    // Bind Vertex Array Object to save bindings for VBO and EBO
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // route data in VBO to the Vertex Attribute at location 0 and enable vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Ubind the VAO
    glBindVertexArray(0);
}


int main(int argc, char **argv) {
//    printf("%lf\n%lf\n%lf", properties.zoom_scale, properties.pos_x, properties.pos_y);
//    return 0;

    // Create window and set context
    GLFWwindow* window = createWindow();
    // compile a shader program
    GLuint shaderProgram = generateShaderProgram();

    // Create OpenGL Objects
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    createBufferObjects(&VAO, &VBO, &EBO);

    // The time that the program has been running
    double time = 0.0f;
    double time_old = 0.0f;
    double time_delta = 0.0f;
    // how much zoom the current time equates to
    double zoom_time = 0.0f;
    // The current zoom level
    double zoom = 1.0f;

    GLint wh_location = glGetUniformLocation(shaderProgram, "wh");
    GLint screen_location = glGetUniformLocation(shaderProgram, "screen");
    GLint center_location = glGetUniformLocation(shaderProgram, "center");

    // upload the screen dimensions to the gpu
    glUseProgram(shaderProgram);
    glUniform2f(screen_location, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
    glUseProgram(0);

    while (!glfwWindowShouldClose(window)) {
        time = glfwGetTime();
        time_delta = time - time_old;
        time_old = time;
        zoom_time = zoom_time + time_delta * properties.zoom_scale;
        zoom = exp2(zoom_time);
        properties.height = 2.0 / zoom;
        properties.width = properties.aspect * properties.height;


        // Clear Screen
        glClearColor(0.1f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // now push it to the shader
        glUniform2f(wh_location, (float)properties.width, (float)properties.height);
        if (properties.moved_center) {
            glUniform2f(center_location, (float)properties.center_x, (float)properties.center_y);
        }

        //bind and unbind VAO if I want to draw more than one object
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        process_input(window);
    }

    glfwTerminate();
    return 0;
}




