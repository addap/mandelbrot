#include "mandelbrot.h"

int main(int argc, char **argv) {
//    printf("%lf\n%lf\n%lf", properties.zoom_scale, properties.pos_x, properties.pos_y);
//    return 0;
    properties_t properties = {
            .zoom_scale = 1.0f,
            .aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT,
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

//    GLint wh_location = glGetUniformLocation(shaderProgram, "wh");
    GLint screen_location = glGetUniformLocation(shaderProgram, "screen");
    GLint cursor_location = glGetUniformLocation(shaderProgram, "cursor");
    GLint border_location = glGetUniformLocation(shaderProgram, "border");
//    GLint center_location = glGetUniformLocation(shaderProgram, "center");

    // upload the screen dimensions to the gpu
    glUseProgram(shaderProgram);
    glUniform2f(screen_location, SCREEN_WIDTH, SCREEN_HEIGHT);
    glUniform1i(border_location, 1);
    glUseProgram(0);

    while (!glfwWindowShouldClose(window)) {
//        time = glfwGetTime();
//        time_delta = time - time_old;
//        time_old = time;
//        zoom_time = zoom_time + time_delta * properties.zoom_scale;
//        zoom = exp2(zoom_time);
//        properties.height = 2.0 / zoom;
//        properties.width = properties.aspect * properties.height;


        // Clear Screen
        glClearColor(0.1f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // now push it to the shader
        glUniform4f(cursor_location, properties.selection_ul_x, properties.selection_ul_y, properties.selection_lr_x, properties.selection_lr_y);

//        glUniform2d(wh_location, properties.width, properties.height);
//        if (properties.moved_center) {
//            glUniform2d(center_location, properties.center_x, properties.center_y);
//        }

        //bind and unbind VAO if I want to draw more than one object
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




