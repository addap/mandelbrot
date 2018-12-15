#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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

const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "void main()\n"
        "{\n"
        "    float aspect = 800.0 / 600.0;\n"
        "    float width = 2.0;\n"
        "    float height = aspect * width;\n"
        "    \n"
        "    vec2 xy = gl_FragCoord.xy;\n"
        "    \n"
        "    xy.x = xy.x / 800.0;\n"
        "    xy.y = xy.y / 600.0;\n"
        "    \n"
        "    float center_x = 0.3750001200618655;\n"
        "    float center_y = -0.2166393884377127;\n"
        "    \n"
        "    float origin_x = center_x - 0.5 * width;\n"
        "    float origin_y = center_y - 0.5 * height;\n"
        "    \n"
        "    xy.x = width * xy.x + origin_x;\n"
        "    xy.y = height * xy.y + origin_y;\n"
        "    \n"
        "    float a, b = 0.0;\n"
        "    float new_a, new_b = 0.0;\n"
        "    float iteration = 0.0;\n"
        "    float max_iter = 100.0;\n"
        "    \n"
        "    while (a * a + b * b <= 4.0 && iteration < max_iter) {\n"
        "        new_a = a * a - b * b + xy.x;\n"
        "        new_b = 2.0 * a * b + xy.y;\n"
        "        \n"
        "        iteration++;\n"
        "        a = new_a;\n"
        "        b = new_b;\n"
        "    }\n"
        "    \n"
        "    if (iteration == max_iter) {\n"
        "        fragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
        "    } else {\n"
        "        fragColor = vec4(iteration / max_iter, 1.0 - iteration / max_iter, 0.7, 1.0);\n"
        "    }\n"
        "}";

void init() {

}

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

int main(int argc, char **argv) {
    glfwSetErrorCallback(error_handler);

    if (glfwInit() == GLFW_FALSE) {
        printf("Failed to init glfw");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window) {
        printf("Failed to open glfw window");
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to load opengl functions");
        exit(1);
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Vertax Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

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

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
    unsigned int shaderProgram;
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
    ///////////////////////

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        //bind and unbind VAO if I want to draw more than one object
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}




