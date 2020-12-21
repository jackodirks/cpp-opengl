#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <list>
#include <functional>
#include <string>

class GlfwWindow {
    private:
        GLFWwindow* window;
        std::list<std::pair<std::function<void(int, int)>, std::function<void(void)>>> resizeCallbackList;

        static void setFramebufferSizeCallback(GLFWwindow* window, int width, int height);
        void windowDidResize(int width, int height);
    public:
        typedef std::list<std::pair<std::function<void(int, int)>, std::function<void(void)>>>::const_iterator ResizeCallbackCookie;
        struct WindowSize {
            int width;
            int height;
        };

        explicit GlfwWindow(int width = 800, int height = 600, const std::string title = "",
                GLFWmonitor* monitor = NULL, GlfwWindow* share = NULL);
        ~GlfwWindow();
        GlfwWindow(const GlfwWindow&) = delete;
        GlfwWindow(GlfwWindow&&) = delete;

        GlfwWindow& operator=(const GlfwWindow&) = delete;
        GlfwWindow& operator=(const GlfwWindow&&) = delete;

        ResizeCallbackCookie registerResizeCallback(std::function<void(int,int)> changeNotify,
                std::function<void(void)> destructNotify);
        void unregisterResizeCallback(const ResizeCallbackCookie& c);
        bool shouldClose(void);
        void swapBuffers(void);
        struct WindowSize getWindowSize(void);
};

#endif //GLFW_WINDOW_HPP
