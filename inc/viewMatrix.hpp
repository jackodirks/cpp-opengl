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
        Vector3 cameraPos;
        Vector3 cameraFront;
        Vector3 cameraUp;
        Matrix4 lookAtMatrix;
        float yaw;
        float pitch;
        const float sensitivity;
        const float moveSpeed;

        Matrix4 getLookAtMatrix(void);
        Vector3 getCameraFront(void);
        void processKeyPress(const int key, const int scancode, const int action, const int mods);
        std::function<void(void)> keyCallbackUnregisterFunction;
    public:
        explicit ViewMatrix(const float sensitivity = 0.1, const float moveSpeed = 2.5);
        //void processMouseMovement(const float xPos, const float yPos);
        void update(void);
        void registerWithGlfwWindow(GlfwWindow& w);
        const float* data(void) noexcept;
};

#endif //VIEW_MATRIX_HPP
