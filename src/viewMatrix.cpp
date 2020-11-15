#include <cmath>

#include "viewMatrix.hpp"

Matrix4 ViewMatrix::getLookAtMatrix(void)
{
    Vector3 center = this->cameraPos + this->cameraFront;
    Vector3 cameraDirection = this->cameraPos - center;
    cameraDirection.normalize();

    Vector3 cameraRight = this->cameraUp.crossProduct(cameraDirection);
    cameraRight.normalize();

    Vector3 cameraUp = cameraDirection.crossProduct(cameraRight);

    Matrix4 axisMat({
        cameraRight.x(),        cameraRight.y(),        cameraRight.z(),        0,
        cameraUp.x(),           cameraUp.y(),           cameraUp.z(),           0,
        cameraDirection.x(),    cameraDirection.y(),    cameraDirection.z(),    0,
        0,                      0,                      0,                      1,
    });

    Matrix4 posMat({
        1,  0,  0,  -this->cameraPos.x(),
        0,  1,  0,  -this->cameraPos.y(),
        0,  0,  1,  -this->cameraPos.z(),
        0,  0,  0,  1,
    });

    return axisMat * posMat;
}

Vector3 ViewMatrix::getCameraFront(void)
{
    float x = std::cos(yaw) * std::cos(pitch);
    float y = std::sin(pitch);
    float z = std::sin(yaw) * std::cos(pitch);
    return Vector3(x,y,z);
}

ViewMatrix::ViewMatrix(const float sensitivity, const float moveSpeed) : sensitivity(sensitivity), moveSpeed(moveSpeed)
{
    this->cameraPos = Vector3(0, 0, 3);
    this->cameraUp = Vector3(0, 1, 0);
    this->yaw = -M_PI / 2;
    this->pitch = 0;
    this->cameraFront = getCameraFront();
    this->lookAtMatrix = getLookAtMatrix();
}

void ViewMatrix::move(const ViewMatrix::MoveDirections direction, const float deltaTime)
{
    float cameraSpeed = this->moveSpeed * deltaTime;
    switch(direction) {
        case MoveDirections::Up:
            this->cameraPos += this->cameraFront * cameraSpeed;
            break;
        case MoveDirections::Down:
            this->cameraPos -= this->cameraFront * cameraSpeed;
            break;
        case MoveDirections::Left:
            {
                Vector3 cross = this->cameraFront.crossProduct(this->cameraUp);
                cross.normalize();
                cross *= cameraSpeed;
                this->cameraPos -= cross;
            }
            break;
        case MoveDirections::Right:
            {
                Vector3 cross = this->cameraFront.crossProduct(this->cameraUp);
                cross.normalize();
                cross *= cameraSpeed;
                this->cameraPos += cross;
            }
            break;
    }
    this->lookAtMatrix = getLookAtMatrix();
}


