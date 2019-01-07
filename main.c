#include "main.h"

int main(int argc, char **argv) {
    properties_t properties;
    init_properties(&properties);

    // Create window and set context
    GLFWwindow* window = createWindow();

    // Create OpenGL Objects
    GLuint VAO = createArrayBuffer();

    // create shader program
    GLuint shaderProgram = generateShaderProgram();

    // create pixel array, curiously I have to declare it as a multi-dimensional array and not as a single array with size HEIGHT*WIDTH*3 even though both result in the same block of contiguous memory
    unsigned char pixels[SCREEN_HEIGHT][SCREEN_WIDTH][3];

    // create texture
    GLuint texture;
    glGenTextures(1, &texture);

    // calculate mandelbrot pixels on cpu
    // bind texture and upload pixel data
//    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint screen_location = glGetUniformLocation(shaderProgram, "screen");
    GLint cursor_location = glGetUniformLocation(shaderProgram, "cursor");
    GLint border_location = glGetUniformLocation(shaderProgram, "border");
    GLint sampler_location = glGetUniformLocation(shaderProgram, "tex");

    // upload the screen dimensions to the gpu
    glUseProgram(shaderProgram);
    glUniform2f(screen_location, SCREEN_WIDTH, SCREEN_HEIGHT);
    glUniform1i(border_location, 1);
    glUniform1i(sampler_location, 0);
    glUseProgram(0);


    while (!glfwWindowShouldClose(window)) {

        process_input(window, &properties);

        // Clear Screen
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // if we have submitted a new selection
        if (properties.submit_selection) {
            properties.submit_selection = 0;
            mandelbrot_rect(&properties, pixels);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)pixels);
        }

        // use shader program
        glUseProgram(shaderProgram);
        glUniform4f(cursor_location, properties.selection_ul_x, properties.selection_ul_y, properties.selection_lr_x, properties.selection_lr_y);
//        printf("%lf, %lf, %lf, %lf\n", properties.selection_ul_x, properties.selection_ul_y, properties.selection_lr_x, properties.selection_lr_y);

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

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




