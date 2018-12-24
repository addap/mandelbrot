#include "main.h"

int main(int argc, char **argv) {
//    printf("%lf\n%lf\n%lf", properties.zoom_scale, properties.pos_x, properties.pos_y);
//    return 0;
    properties_t properties = {
            .aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT
    };

    // Create window and set context
    GLFWwindow* window = createWindow();

    // Create OpenGL Objects
    GLuint VAO = createArrayBuffer();

    // create shader program
    GLuint shaderProgram = generateShaderProgram();

    // create pixel array
    unsigned char pixels[SCREEN_WIDTH*SCREEN_HEIGHT*3];
    // create texture
    GLuint texture;
    glGenTextures(1, &texture);

    // calculate mandelbrot pixels on cpu
    mandelbrot_rect(properties.origin_x, properties.origin_y, properties.width, properties.height, &pixels);
    // bind texture and upload pixel data
    glBindTexture(GL_TEXTURE_RECTANGLE, texture);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)&pixels);

    GLint screen_location = glGetUniformLocation(shaderProgram, "screen");
    GLint cursor_location = glGetUniformLocation(shaderProgram, "cursor");
    GLint border_location = glGetUniformLocation(shaderProgram, "border");

    // upload the screen dimensions to the gpu
    glUseProgram(shaderProgram);
    glUniform2f(screen_location, SCREEN_WIDTH, SCREEN_HEIGHT);
    glUniform1i(border_location, 1);
    glUseProgram(0);

    properties.height = 2.0;
    properties.width = properties.aspect * properties.height;

    while (!glfwWindowShouldClose(window)) {

        process_input(window, &properties);

        // Clear Screen
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind texture
        glBindTexture(GL_TEXTURE_RECTANGLE, texture);
        // use shader program
        glUseProgram(shaderProgram);
        glUniform4f(cursor_location, properties.selection_ul_x, properties.selection_ul_y, properties.selection_lr_x, properties.selection_lr_y);
//        printf("%lf, %lf, %lf, %lf\n", properties.selection_ul_x, properties.selection_ul_y, properties.selection_lr_x, properties.selection_lr_y);

        //bind VAO to draw the triangles over the whole screen
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}




