#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const char *vertexShaderSource =
        "#version 400 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}";
/*
const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}";*/

const char* fragmentShaderSourceFile = "../mandelbrot.glsl";


void framebuffer_size_callback(GLFWwindow* window, int height, int width) {
    glViewport(0, 0, height, width);
}

void error_handler(int i, const char* err_str) {
    printf("%d, %s\n", i, err_str);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

GLFWwindow* createWindow() {
    glfwSetErrorCallback(error_handler);

    if (glfwInit() == GLFW_FALSE) {
        printf("Failed to init glfw");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window) {
        printf("Failed to open glfw window");
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to load opengl functions");
        exit(1);
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

const char* readFile(const char *file_url) {
    FILE* file = fopen(file_url, "r");

    if (!file) {
        printf("Could not open file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);

    size_t size = (size_t)ftell(file);
    rewind(file);
    char* buffer = calloc(1, size + 1);
    size_t pos = 0;

    if (!buffer) {
        printf("Could not create buffer");
    }

    while (1) {
        if (fread(&(buffer[pos]), size - pos, 1, file)) {
            // we are finished
            fclose(file);
            break;
        } else {
            printf("Have to read more");
            // else we did not read the whole file
            pos = strlen(buffer);
            fseek(file, pos, SEEK_SET);
        }

    }
    return buffer;
}

GLuint generateShaderProgram() {
    //Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        char *info_log = malloc(512);
        glGetShaderInfoLog(vertexShader, 512, NULL, info_log);
        printf("Vertex shader compilation error: %s", info_log);
    }
    ///////////////////////
    // Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = readFile(fragmentShaderSourceFile);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        char *info_log = malloc(512);
        glGetShaderInfoLog(fragmentShader, 512, NULL, info_log);
        printf("Fragment shader compilation error: %s", info_log);
    }
    ////////////////////////
    // Shader Program
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        char *info_log = malloc(512);
        glGetProgramInfoLog(vertexShader, 512, NULL, info_log);
        printf("Program linking error: %s", info_log);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(fragmentShaderSource);
    ///////////////////////
    return shaderProgram;
}

int main(int argc, char **argv) {
    // Create window and set context
    GLFWwindow* window = createWindow();
    // compile a shader program
    GLuint shaderProgram = generateShaderProgram();


    float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
            0, 1, 2,
            1, 3, 2
    };

    // Create OpenGL Objects
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind Vertex Array Object to save bindings for VBO and EBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // route data in VBO to the Vertex Attribute at location 0 and enable vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Ubind the VAO
    glBindVertexArray(0);

    float time = 0.0f;
    int timeLocation = glGetUniformLocation(shaderProgram, "time");

    while (!glfwWindowShouldClose(window)) {
        time = (float)glfwGetTime();
        process_input(window);

        // Clear Screen
        glClearColor(0.1f, 0.0f, 0.5f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform1f(timeLocation, time);

        //bind and unbind VAO if I want to draw more than one object
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




