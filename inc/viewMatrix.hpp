#ifndef VIEW_MATRIX_HPP
#define VIEW_MATRIX_HPP

#include "matrix4.hpp"
#include "vector3.hpp"

class ViewMatrix {
    private:
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
    public:
        enum class MoveDirections {
            Up,
            Down,
            Left,
            Right,
        };
        ViewMatrix(const float sensitivity = 0.1, const float moveSpeed = 2.5);
        void move(const ViewMatrix::MoveDirections direction, const float deltaTime);
        void processMouseMovement(const float xPos, const float yPos);
        const float* data(void) noexcept;
};

#endif //VIEW_MATRIX_HPP
