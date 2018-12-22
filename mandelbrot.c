#include "mandelbrot.h"

Properties properties = {
        .zoom_scale = 1.0f,
        .x_pos = 0.0f,
        .y_pos = 0.0f
};

int pressed_states[3];
int UP_STATE = 0;
int DOWN_STATE = 1;
int E_STATE = 2;

float zoom_speed = 0.0;

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !pressed_states[UP_STATE]) {
        if (zoom_speed <= 10.0) {
            zoom_speed++;
        } else {
            zoom_speed = 10;
        }
        pressed_states[UP_STATE] = 1;
    } else {
        pressed_states[UP_STATE] = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !pressed_states[DOWN_STATE]) {
        if (zoom_speed >= 1) {
            zoom_speed++;
        } else {
            zoom_speed = 10;
        }
    }

}

void createBufferObjects (GLuint* VAO, GLuint* VBO, GLuint* EBO) {
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
//    printf("%lf\n%lf\n%lf", properties.zoom_scale, properties.x_pos, properties.y_pos);
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

    double x_pos_new, x_pos_scaled;
    double y_pos_new, y_pos_scaled;

    double height, width;
    double aspect = (double)SCREEN_WIDTH / SCREEN_HEIGHT;
    GLint wh_location = glGetUniformLocation(shaderProgram, "wh");
    GLint screen_location = glGetUniformLocation(shaderProgram, "screen");

    glUseProgram(shaderProgram);
    glUniform2f(screen_location, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
    glUseProgram(0);

    double center_x = 0;
    double center_y = 0;
    GLint center_location = glGetUniformLocation(shaderProgram, "center");

    int pressed = 0;
    int zoomed = 0;

    while (!glfwWindowShouldClose(window)) {
        time = glfwGetTime();
        time_delta = time - time_old;
        time_old = time;
        zoom_time = zoom_time + time_delta * properties.zoom_scale;
        zoom = exp2(zoom_time);
        height = 2.0 / zoom;
        width = aspect * height;


        // Clear Screen
        glClearColor(0.1f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // now push it to the shader
        glUniform2f(wh_location, (float)width, (float)height);
        glUniform2f(center_location, (float)center_x, (float)center_y);

        //bind and unbind VAO if I want to draw more than one object
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        process_input(window);

        /// CALC NEW CENTER
        // get new cursor position
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (!pressed) {
                pressed = 1;

                glfwGetCursorPos(window, &x_pos_new, &y_pos_new);
                if (x_pos_new >= 0.0f && x_pos_new <= SCREEN_WIDTH && y_pos_new >= 0.0f && y_pos_new <= SCREEN_HEIGHT) {
                    properties.x_pos = x_pos_new;
                    properties.y_pos = y_pos_new;

                    // scale to screen coordinates
                    x_pos_scaled = properties.x_pos / SCREEN_WIDTH;
                    y_pos_scaled = properties.y_pos / SCREEN_HEIGHT;

                    // make relative to center
                    x_pos_scaled = x_pos_scaled - 0.5f;
                    y_pos_scaled = y_pos_scaled - 0.5f;

                    // calculate next center
                    center_x = x_pos_scaled * width + center_x;
                    center_y = - y_pos_scaled * height + center_y;
                }
//            printf("Cursor is at %lf, %lf\n", x_pos_new, y_pos_new);
//            printf("WINDOWCursor is at %lf, %lf\n", x_pos, y_pos);
            }
        } else {
            pressed = 0;
        }
    }

    glfwTerminate();
    return 0;
}




