#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <list>
#include <functional>
#include <string>

/**This class manages a GLFWwindow. Additionaly, it can receive callbacks from glfw and distribute them to subscribers.
 * Its internals are heavily based on the GLFWwindow internals, so refer to their docs for more information.*/
class GlfwWindow {
    private:
        GLFWwindow* window;
        std::list<std::pair<std::function<void(int, int)>, std::function<void(void)>>> resizeCallbackList;
        std::list<std::pair<std::function<void(int, int, int, int)>, std::function<void(void)>>> keyCallbackList;
        std::list<std::pair<std::function<void(bool)>, std::function<void(void)>>> focusCallbackList;
        std::list<std::pair<std::function<void(double, double)>, std::function<void(void)>>> scrollCallbackList;

        /**This function is registered using glfwSetFramebufferSizeCallback as callback.*/
        static void resizeCallbackFun(GLFWwindow* window, int width, int height);
        /**This function is registered using glfwSetKeyCallback as callback.*/
        static void keyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods);
        /**This function is registered using glfwSetWindowFocusCallback as callback.*/
        static void windowFocusFun(GLFWwindow* window, int focused);
        /**This function is registered using glfwSetScrollCallback as callback*/
        static void scrollCallbackFun(GLFWwindow* window, double xoffset, double yoffset);
    public:
        /**See GlfwWindow::getWindowSize.*/
        struct WindowSize {
            int width;
            int height;
        };
        /**See GlfwWindow::getCursorPosition*/
        struct CursorPosition {
            double xpos;
            double ypos;
        };
        /**See GlfwWindow::setCursorMode*/
        enum class MouseCursorMode {
            normal,
            hidden,  /**Normal, but hidden*/
            disabled /**No mouse cursor at all*/
        };

        /**Default constructor. Explicit to prevent GlfwWindow a = 5 shenanigans
         * See the documentation of glfwCreateWindow for more information.
         * @warning This constructor might throw.*/
        explicit GlfwWindow(int width = 800, int height = 600, const std::string title = "",
                GLFWmonitor* monitor = nullptr, GlfwWindow* share = nullptr);
        /**The constructor informs all registered callbacks that it no longer exists.
         * Also cleans up the GlfwWindow.*/
        ~GlfwWindow();

        /**Copy constructor is deleted because the GlfwWindow can only be terminated once.*/
        GlfwWindow(const GlfwWindow&) = delete;
        /**Move constructor is deleted because the unregister functions returned by register*Callback would be invalidated.*/
        GlfwWindow(GlfwWindow&&) = delete;
        /**Copy assignment is deleted because the GlfwWindow can only be terminated once.*/
        GlfwWindow& operator=(const GlfwWindow&) = delete;
        /**Move assignment is deleted because the unregister functions returned by register*Callback would be invalidated.*/
        GlfwWindow& operator=(const GlfwWindow&&) = delete;

        /**@brief Used to register an std::function which will be called after GLFW calls the frameBufferSizeCallback.
         *
         * It is a somewhat complex setup which utilizes three functions, two for administration and one as the actual callback.
         * If you no longer whish to receive frameBufferSizeCallback events, you need to call the function returned from this one.
         * If this object destructs, it will call the destructNotify. After the destructNotify was called, you are no longer allowed to call the function returned from this method.
         * See also: GLFWframebuffersizefun.
         *
         * @param changeNotify This function is called when frameBufferSizeCallback fires. It needs to accept two parameters: int width and int height.
         * @param destructNotify After this function has been called, the function returned from here can no longer be called, and the changeNotify function will never be called again.
         * @return A function which can be used to inform this GlfwWindow that the related changeNotify may never be called again.
         * @warning Do not call the function returned from this function after the destructNotify function has been called. Doing this will lead to undefined behaviour.
         * @warning If the function returned from this method is called more than once, it will cause undefined behaviour.
         */
        std::function<void(void)> registerResizeCallback(std::function<void(int,int)> changeNotify, std::function<void(void)> destructNotify);
        /**@brief Used to register an std::function which will be called after GLFW calls the keyCallback.
         *
         * It is a somewhat complex setup which utilizes three functions, two for administration and one as the actual callback.
         * If you no longer whish to receive keyCallback events, you need to call the function returned from this one.
         * If this object destructs, it will call the destructNotify. After the destructNotify was called, you are no longer allowed to call the function returned from this method.
         * See also: GLFWkeyfun
         *
         * @param changeNotify This function is called when keyCallback fires. It needs to accept four parameters: int key, int scancode, int action and int mods
         * @param destructNotify After this function has been called, the function returned from here can no longer be called, and the changeNotify function will never be called again.
         * @return A function which can be used to inform this GlfwWindow that the related changeNotify may never be called again.
         * @warning Do not call the function returned from this function after the destructNotify function has been called. Doing this will lead to undefined behaviour.
         * @warning If the function returned from this method is called more than once, it will cause undefined behaviour.
         */
        std::function<void(void)> registerKeyCallback(std::function<void(int,int,int,int)> changeNotify, std::function<void(void)> destructNotify);
        /**@brief Used to register an std::function which will be called after GLFW calls the windowFocusCallback.
         *
         * It is a somewhat complex setup which utilizes three functions, two for administration and one as the actual callback.
         * If you no longer whish to receive keyCallback events, you need to call the function returned from this one.
         * If this object destructs, it will call the destructNotify. After the destructNotify was called, you are no longer allowed to call the function returned from this method.
         * See also: GLFWwindowfocusfun.
         *
         * @param changeNotify This function is called when windowFocusCallback fires. It needs to accept one parameter: bool focused.
         * @param destructNotify After this function has been called, the function returned from here can no longer be called, and the changeNotify function will never be called again.
         * @return A function which can be used to inform this GlfwWindow that the related changeNotify may never be called again.
         * @warning Do not call the function returned from this function after the destructNotify function has been called. Doing this will lead to undefined behaviour.
         * @warning If the function returned from this method is called more than once, it will cause undefined behaviour.
         */
        std::function<void(void)> registerWindowFocusCallback(std::function<void(bool)> changeNotify, std::function<void(void)> destructNotify);
        /**@brief Used to register an std::function which will be called after GLFW calls the scrollCallback.
         *
         * It is a somewhat complex setup which utilizes three functions, two for administration and one as the actual callback.
         * If you no longer whish to receive keyCallback events, you need to call the function returned from this one.
         * If this object destructs, it will call the destructNotify. After the destructNotify was called, you are no longer allowed to call the function returned from this method.
         * See also: GLFWscrollfun.
         *
         * @param changeNotify This function is called when windowFocusCallback fires. It needs to accept one parameter: bool focused.
         * @param destructNotify After this function has been called, the function returned from here can no longer be called, and the changeNotify function will never be called again.
         * @return A function which can be used to inform this GlfwWindow that the related changeNotify may never be called again.
         * @warning Do not call the function returned from this function after the destructNotify function has been called. Doing this will lead to undefined behaviour.
         * @warning If the function returned from this method is called more than once, it will cause undefined behaviour.
         */
        std::function<void(void)> registerScrollCallback(std::function<void(double, double)> changeNotify, std::function<void(void)> destructNotify);
        /**Indicates that the window has received a signal that it should close. See glfwWindowShouldClose.*/
        bool shouldClose(void);
        /**See glfwSwapBuffers.*/
        void swapBuffers(void);
        /**See glfwGetWindowSize.*/
        const struct WindowSize getWindowSize(void);
        /**See glfwGetCursorPos*/
        const struct CursorPosition getCursorPosition(void);
        /**Set a new mouse cursor mode (normal, hidden or disabled).
         * See also: glfwSetInputMode and GlfwWindow::MouseCursorMode*/
        void setCursorMode(MouseCursorMode newMode);
        /**Check if this GlfwWindow has focus. See glfwGetWindowAttrib*/
        const bool windowHasFocus(void);
};

#endif //GLFW_WINDOW_HPP
