#include <iostream>
#include <cmath>
#include <sstream>

#include "viewMatrix.hpp"
#include "perspectiveProjectionMatrix.hpp"
#include "orthographicProjectionMatrix.hpp"
#include "openglMatrix.hpp"
#include "shaderProgram.hpp"
#include "textRenderer.hpp"
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

    if (!glfwInit()) {
        std::cerr << "glfwInit failed";
        std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GlfwWindow window = []() -> GlfwWindow {
        try {
            return GlfwWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
        } catch (const std::runtime_error &e) {
            std::cerr << "An error occured during construction of GlfwWindow: " << e.what();
            std::exit(EXIT_FAILURE);
        }
    }();
    window.setCursorMode(GlfwWindow::MouseCursorMode::disabled);
    glEnable(GL_DEPTH_TEST);
    // Enable blending. This is required to properly display the text.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    viewMatrix.registerWithGlfwWindow(window);
    GlfwWindow::WindowSize size = window.getWindowSize();
    PerspectiveProjectionMatrix projectionMatrix(DEGREES_TO_RADIANS(45.0), size.width, size.height, 0.1, 100);
    OpenGlMatrix modelMatrix;
    modelMatrix.setScale(0.5, 0.5, 0.5);
    projectionMatrix.registerGlfwWindow(window);

    TextRenderer tRen = TextRenderer("serif", 0, 24);
    OrthographicProjectionMatrix ortMat = OrthographicProjectionMatrix(0, size.width, 0, size.height, -50, 50);
    ortMat.registerGlfwWindow(window);

    float lastTime = glfwGetTime();
    while(!window.shouldClose()) {
        // Set the background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the viewMatrix
        viewMatrix.update();
        // Draw the cube
        shader.use();
        // Copy the matrices
        modelMatrix.setTranslate();
        shader.setUniformMatrix4v("model", 1, true, modelMatrix.data());
        shader.setUniformMatrix4v("view", 1, true, viewMatrix.data());
        shader.setUniformMatrix4v("projection", 1, true, projectionMatrix.data());
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelMatrix.setTranslate(1.5);
        shader.setUniformMatrix4v("model", 1, true, modelMatrix.data());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelMatrix.setTranslate(-1.5);
        shader.setUniformMatrix4v("model", 1, true, modelMatrix.data());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelMatrix.setTranslate(0, -1.5);
        shader.setUniformMatrix4v("model", 1, true, modelMatrix.data());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelMatrix.setTranslate(0, 1.5);
        shader.setUniformMatrix4v("model", 1, true, modelMatrix.data());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelMatrix.addLateRotate(0,0, 2*M_PI/900);
        modelMatrix.addRotate(0, 2*M_PI/900, 0);
        Vector3 textColor = {0.5, 0.8, 0.2};


        const struct GlfwWindow::CursorPosition cPos = window.getCursorPosition();
        std::ostringstream stream;
        stream << "Mouse cursor position: (" << cPos.xpos << ", " << cPos.ypos << ")" << ".";
        tRen.renderText(stream.str(), 25.0f, 75.0f, 1.0f, textColor, ortMat);

        const bool hasFocus = window.windowHasFocus();
        stream = std::ostringstream();
        stream << "Window has focus: " << (hasFocus ? "yes" : "no") << ".";
        tRen.renderText(stream.str(), 25.0f, 50.0f, 1.0f, textColor, ortMat);

        float curTime = glfwGetTime();
        float timeDiff = curTime - lastTime;
        lastTime = curTime;
        float fps = 1/timeDiff;
        stream = std::ostringstream();
        stream << "Current FPS: " << fps << ".";
        tRen.renderText(stream.str(), 25.0f, 25.0f, 1.0f, textColor, ortMat);
        window.swapBuffers();
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
