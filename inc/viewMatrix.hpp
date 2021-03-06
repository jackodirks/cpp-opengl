#ifndef VIEW_MATRIX_HPP
#define VIEW_MATRIX_HPP

#include "matrix4.hpp"
#include "vector3.hpp"
#include "glfwWindow.hpp"

class ViewMatrix {
    private:
        bool upActive;
        bool downActive;
        bool leftActive;
        bool rightActive;
        Matrix4 lookAtMatrix;
        const float sensitivity;
        const float moveSpeed;
        GlfwWindow* glfwWindow;
        bool windowHasFocus;
        bool mouseCursorCalibrated;
        std::function<void(void)> keyCallbackUnregisterFunction;
        std::function<void(void)> focusCallbackUnregisterFunction;
        float prevTime;
        double prevXpos;
        double prevYpos;
        double yaw;
        double pitch;
        const double pitchLimit;
        Vector3 cameraPos;

        void processKeyPress(const int key, const int scancode, const int action, const int mods);
        void processWindowFocus(const bool focused);
        void unregisterGlfwWindow(void);
        Vector3 getCameraFront(const float pitch, const float yaw) const;
        Matrix4 getLookAtMatrix(const Vector3 &worldUp, const Vector3 &cameraDirection, const Vector3 &cameraPos) const;
    public:
        explicit ViewMatrix(const float sensitivity = 0.001, const float moveSpeed = 2.5);
        ~ViewMatrix(void);

        ViewMatrix(const ViewMatrix&) = delete;
        ViewMatrix& operator=(const ViewMatrix&) = delete;

        void update(void);
        void registerWithGlfwWindow(GlfwWindow& w);
        const float* data(void) const noexcept;
};

#endif //VIEW_MATRIX_HPP
