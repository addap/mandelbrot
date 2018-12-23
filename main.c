#include "main.h"

int main(int argc, char **argv) {
//    printf("%lf\n%lf\n%lf", properties.zoom_scale, properties.pos_x, properties.pos_y);
//    return 0;
    properties_t properties = {
            .zoom_scale = 1.0f,
            .aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT
    };

    // Create window and set context
    GLFWwindow* window = createWindow();
    // compile a shader program
    GLuint shaderProgram = generateShaderProgram();

    // Create OpenGL Objects
    GLuint VAO = createArrayBuffer();

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
    glUniform2d(screen_location, (double)SCREEN_WIDTH, (double)SCREEN_HEIGHT);
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
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // now push it to the shader
        glUniform2d(wh_location, properties.width, properties.height);
        if (properties.moved_center) {
            glUniform2d(center_location, properties.center_x, properties.center_y);
        }
//        printf("w: %lf, h: %lf, c: (%lf, %lf)\n", properties.width, properties.height, properties.center_x, properties.center_y);

        //bind VAO to draw the triangles over the whole screen
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        process_input(window, &properties);
    }

    glfwTerminate();
    return 0;
}




