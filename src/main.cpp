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
    ShaderProgram lightCubeShader = []() -> ShaderProgram {
        try {
            return ShaderProgram("shaders/vertex.vert", "shaders/fragmentLightsource.frag");
        } catch (const std::runtime_error &e) {
            std::cerr << "An error occured during construction of OpenGL shader lightSourceShader: " << e.what();
            std::exit(EXIT_FAILURE);
        }
    }();
    ShaderProgram lightingShader = []() -> ShaderProgram {
        try {
            return ShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");
        } catch (const std::runtime_error &e) {
            std::cerr << "An error occured during construction of OpenGL shader lightingShader: " << e.what();
            std::exit(EXIT_FAILURE);
        }
    }();

    GLuint VBO, cubeVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ViewMatrix viewMatrix;
    viewMatrix.registerWithGlfwWindow(window);
    GlfwWindow::WindowSize wSize = window.getWindowSize();
    PerspectiveProjectionMatrix projectionMatrix(DEGREES_TO_RADIANS(45.0), wSize.width, wSize.height, 0.1, 100);
    OpenGlMatrix lightCubeModel;
    OpenGlMatrix cubeModel;
    lightCubeModel.setScale(0.2, 0.2, 0.2);
    lightCubeModel.setTranslate(1.2, 1.0, 2.0);
    projectionMatrix.registerGlfwWindow(window);

    TextRenderer tRen = TextRenderer("serif", 0, 24);
    OrthographicProjectionMatrix ortMat = OrthographicProjectionMatrix(0, wSize.width, 0, wSize.height, -50, 50);
    ortMat.registerGlfwWindow(window);

    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);

    float lastTime = glfwGetTime();
    //const Vector3 textColor = {0.5, 0.8, 0.2};
    while(!window.shouldClose()) {
        // Set the background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the viewMatrix
        viewMatrix.update();
        // Draw the colored cube
        lightingShader.use();
        lightingShader.setUniform3f("objectColor", 1.0, 0.5, 0.31);
        lightingShader.setUniform3f("lightColor", 1, 1, 1);
        lightingShader.setUniformMatrix4v("model", 1, true, cubeModel.data());
        lightingShader.setUniformMatrix4v("view", 1, true, viewMatrix.data());
        lightingShader.setUniformMatrix4v("projection", 1, true, projectionMatrix.data());
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the light cube
        lightCubeShader.use();
        lightingShader.setUniformMatrix4v("model", 1, true, lightCubeModel.data());
        lightingShader.setUniformMatrix4v("view", 1, true, viewMatrix.data());
        lightingShader.setUniformMatrix4v("projection", 1, true, projectionMatrix.data());
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const GlfwWindow::WindowSize size = window.getWindowSize();
        const struct GlfwWindow::CursorPosition cPos = window.getCursorPosition();
        const bool hasFocus = window.windowHasFocus();
        float curTime = glfwGetTime();
        float timeDiff = curTime - lastTime;
        lastTime = curTime;
        float fps = 1/timeDiff;

        std::ostringstream stream;
        stream << "Mouse cursor position: (" << cPos.xpos << ", " << cPos.ypos << ")" << ".\n";
        stream << "Window has focus: " << (hasFocus ? "yes" : "no") << ".\n";
        stream << "Current FPS: " << fps << ".\n";
        tRen.renderText(ortMat, stream.str(), 0, size.height, 1.0f, 0, TextRenderer::VerticalAlignment::top,
                        TextRenderer::HorizontalAlignment::left);

        stream.str(std::string());
        stream << "GL_VENDOR: " << vendor << "\n";
        stream << "GL_RENDERER: " << renderer << "\n";
        tRen.renderText(ortMat, stream.str(), size.width, size.height, 1.0f, size.width/2,
                        TextRenderer::VerticalAlignment::top,
                        TextRenderer::HorizontalAlignment::right);

        window.swapBuffers();
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
