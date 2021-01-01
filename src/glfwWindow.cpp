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
}

GlfwWindow::~GlfwWindow()
{
    for (std::pair<std::function<void(int, int)>, std::function<void(void)>> p : resizeCallbackList) {
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

bool GlfwWindow::shouldClose(void)
{
    return glfwWindowShouldClose(window);
}

void GlfwWindow::swapBuffers(void)
{
    glfwSwapBuffers(window);
}

struct GlfwWindow::WindowSize GlfwWindow::getWindowSize(void)
{
    struct GlfwWindow::WindowSize windowSize;
    glfwGetWindowSize(window, &windowSize.width, &windowSize.height);
    return windowSize;
}
