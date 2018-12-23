//
// Created by adrian on 21.12.18.
//

#include "shader.h"

const char* vertexShaderSourceFile = "../vertex-shader-id.glsl";
const char* fragmentShaderSourceFile = "../escape-algorithm-colors.glsl";

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
    const char* vertexShaderSource = readFile(vertexShaderSourceFile);
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
    free((void*)fragmentShaderSource);
    ///////////////////////
    return shaderProgram;
}