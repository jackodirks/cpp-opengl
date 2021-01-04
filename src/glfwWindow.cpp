#include <sstream>

#include "glfwWindow.hpp"

void GlfwWindow::resizeCallbackFun(GLFWwindow *window, int width, int height)
{
    GlfwWindow *glfwWindow = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
    for (std::pair<std::function<void(int, int)>, std::function<void(void)>> p : glfwWindow->resizeCallbackList) {
        std::get<0>(p)(width, height);
    }
    glViewport(0, 0, width, height);
}

void GlfwWindow::keyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GlfwWindow *glfwWindow = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
    for (std::pair<std::function<void(int, int, int, int)>, std::function<void(void)>> p : glfwWindow->keyCallbackList) {
        std::get<0>(p)(key, scancode, action, mods);
    }
}

GlfwWindow::GlfwWindow(int width, int height, const std::string title, GLFWmonitor* monitor, GlfwWindow* share)
{
    GLFWwindow *s = nullptr;
    if (share != nullptr)
        s = share->window;
    window = glfwCreateWindow(width, height, title.c_str(), monitor, s);
    if (window == nullptr) {
        const char* errDesc;
        int err = glfwGetError(&errDesc);
        std::ostringstream errStream;
        if (errDesc == nullptr)
            errDesc = "<glfwGetError set errDesc to NULL>";
        errStream << "glfwCreateWindow failed: " << err << " " << errDesc;
        throw std::runtime_error(errStream.str());
    }
    glfwMakeContextCurrent(window);
    const char* errDesc;
    int err = glfwGetError(&errDesc);
    if (err != GLFW_NO_ERROR) {
        if (errDesc == nullptr)
            errDesc = "<glfwGetError set errDesc to NULL>";
        std::ostringstream errStream;
        errStream << "glfwMakeContextCurrent failed: " << err << " " << errDesc;
        throw std::runtime_error(errStream.str());
    }
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("gladLoadGLLoader failed");
    }
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, GlfwWindow::resizeCallbackFun);
    glfwSetKeyCallback(window, GlfwWindow::keyCallbackFun);
}

GlfwWindow::~GlfwWindow()
{
    for (auto p : resizeCallbackList) {
        std::get<1>(p)();
    }
    for (auto p : keyCallbackList) {
        std::get<1>(p)();
    }
    glfwDestroyWindow(window);
}

std::function<void(void)> GlfwWindow::registerResizeCallback(std::function<void(int,int)> changeNotify, std::function<void(void)> destructNotify)
{
    resizeCallbackList.push_front(std::make_pair(changeNotify, destructNotify));
    return [cookie = resizeCallbackList.cbegin(), this]() {
        static bool called = false;
        if (called)
            throw std::runtime_error("This function can only be called once.");
        called = true;
        this->resizeCallbackList.erase(cookie);
    };
}


std::function<void(void)> GlfwWindow::registerKeyCallback(std::function<void(int,int,int,int)> changeNotify, std::function<void(void)> destructNotify)
{
    keyCallbackList.push_front(std::make_pair(changeNotify, destructNotify));
    return [cookie = keyCallbackList.cbegin(), this]() {
        static bool called = false;
        if (called)
            throw std::runtime_error("This function can only be called once.");
        called = true;
        this->keyCallbackList.erase(cookie);
    };
}

bool GlfwWindow::shouldClose(void)
{
    return glfwWindowShouldClose(window);
}

void GlfwWindow::swapBuffers(void)
{
    glfwSwapBuffers(window);
}

const struct GlfwWindow::WindowSize GlfwWindow::getWindowSize(void)
{
    struct GlfwWindow::WindowSize windowSize;
    glfwGetWindowSize(window, &windowSize.width, &windowSize.height);
    return windowSize;
}

const struct GlfwWindow::CursorPosition GlfwWindow::getCursorPosition(void)
{
    struct GlfwWindow::CursorPosition cursorPos;
    glfwGetCursorPos(window, &cursorPos.xpos, &cursorPos.ypos);
    return cursorPos;
}

void GlfwWindow::setCursorMode(GlfwWindow::MouseCursorMode newMode)
{
    int value = GLFW_CURSOR_NORMAL;
    switch(newMode) {
        case GlfwWindow::MouseCursorMode::normal:
            value = GLFW_CURSOR_NORMAL;
            break;
        case GlfwWindow::MouseCursorMode::hidden:
            value = GLFW_CURSOR_HIDDEN;
            break;
        case GlfwWindow::MouseCursorMode::disabled:
            value = GLFW_CURSOR_DISABLED;
            break;
    }
    glfwSetInputMode(window, GLFW_CURSOR, value);
}
