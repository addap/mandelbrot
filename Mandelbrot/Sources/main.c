#include "main.h"

int start_cpu();
int start_gpu();

void usage (char **argv) {
    printf("usage: %s cpu | gpu\n", argv[0]);
    exit(1);
}

int main(int argc, char **argv) {

    if (argc < 2) {
        usage(argv);
    }

    if (strcmp(argv[1], "cpu") == 0) {
        return start_cpu();
    } else if (strcmp(argv[1], "gpu") == 0) {
        return start_gpu();
    } else {
        usage(argv);
    }
}

int start_cpu() {
    properties_t properties = {
            .aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT,
            .submit_selection = 1,
    };

    properties.height = 2.0,
            properties.width = properties.aspect * properties.height;
    properties.origin_x = - 0.5 - properties.width / 2;
    properties.origin_y = properties.height / 2;


    // Create window and set context
    GLFWwindow* window = createWindow();

    // Create OpenGL Objects
    GLuint VAO = createArrayBuffer();

    // create shader program
    GLuint shaderProgram = generateShaderProgram(1);

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

        process_input_cpu(window, &properties);

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

int start_gpu() {
    properties_t properties = {
            .aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT,
            .submit_selection = 1,
            .zoom_scale = 1,
    };

    properties.height = 2.0,
    properties.width = properties.aspect * properties.height;
    properties.origin_x = - 0.5 - properties.width / 2;
    properties.origin_y = properties.height / 2;


    // Create window and set context
    GLFWwindow* window = createWindow();

    // Create OpenGL Objects
    GLuint VAO = createArrayBuffer();

    // create shader program
    GLuint shaderProgram = generateShaderProgram(0);

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
        process_input_gpu(window, &properties);
    }

    glfwTerminate();
    return 0;
}



