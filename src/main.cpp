#include <iostream>
#include <cmath>

#include "viewMatrix.hpp"
#include "perspectiveProjectionMatrix.hpp"
#include "openglMatrix.hpp"
#include "shaderProgram.hpp"
#include "GLFW/glfw3.h"

#define DEGREES_TO_RADIANS(degrees) ((degrees) * M_PI / 180.0)

static float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GlfwWindow window = []() -> GlfwWindow {
        try {
            return GlfwWindow(800, 600, "LearnOpenGL", NULL, NULL);
        } catch (const std::runtime_error &e) {
            std::cerr << "An error occured during construction of GlfwWindow: " << e.what();
            std::exit(EXIT_FAILURE);
        }
    }();
    glEnable(GL_DEPTH_TEST);
    ShaderProgram shader = []() -> ShaderProgram {
        try {
            return ShaderProgram("shaders/vertex.vert", "shaders/fragmentLightsource.frag");
        } catch (const std::runtime_error &e) {
            std::cerr << "An error occured during construction of OpenGL shader: " << e.what();
            std::exit(EXIT_FAILURE);
        }
    }();
    GLuint VBO, cubeVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ViewMatrix viewMatrix;
    PerspectiveProjectionMatrix projectionMatrix(DEGREES_TO_RADIANS(45.0), 800, 600, 0.1, 100);
    OpenGlMatrix modelMatrix;
    projectionMatrix.registerWindowResizeCallback(window);

    while(!window.shouldClose()) {
        // Set the background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the cube
        shader.use();
        // Copy the matrices
        shader.setUniformMatrix4v("model", 1, true, modelMatrix.data());
        shader.setUniformMatrix4v("view", 1, true, viewMatrix.data());
        shader.setUniformMatrix4v("projection", 1, true, projectionMatrix.data());
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window.swapBuffers();
        glfwPollEvents();
    }
    return 0;
}
