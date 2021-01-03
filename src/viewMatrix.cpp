#include <cmath>

#include "viewMatrix.hpp"

void ViewMatrix::processKeyPress(const int key, const int scancode, const int action, const int mods)
{
    (void)scancode;
    (void)mods;

    // The action might be one of many, but these are the two we are interested in.
    if (action != GLFW_RELEASE && action != GLFW_PRESS)
        return;

    bool becomesSet = (action == GLFW_PRESS);
    switch(key){
        case GLFW_KEY_W:
            upActive = becomesSet;
            break;
        case GLFW_KEY_S:
            downActive = becomesSet;
            break;
        case GLFW_KEY_A:
            leftActive = becomesSet;
            break;
        case GLFW_KEY_D:
            rightActive = becomesSet;
            break;
        default:
            break;
    }
}

Matrix4 ViewMatrix::getLookAtMatrix(void)
{
    Vector3 center = this->cameraPos + this->cameraFront;
    Vector3 cameraDirection = this->cameraPos - center;
    cameraDirection.normalize();

    Vector3 cameraRight = Vector3::crossProduct(cameraUp, cameraDirection);
    cameraRight.normalize();

    Vector3 cameraUp = Vector3::crossProduct(cameraDirection, cameraRight);

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
    cameraPos = Vector3(0, 0, 3);
    cameraUp = Vector3(0, 1, 0);
    yaw = -M_PI / 2;
    pitch = 0;
    cameraFront = getCameraFront();
    lookAtMatrix = getLookAtMatrix();
    upActive = downActive = leftActive = rightActive = false;
    keyCallbackUnregisterFunction = 0;
}

void ViewMatrix::update(void)
{
    static float prevTime = 0;
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - prevTime;
    prevTime = currentTime;

    // TODO: camera orientation (mouse movement) must come first.
    // cameraFront = getCameraFront();

    float cameraSpeed = this->moveSpeed * deltaTime;
    Vector3 leftRightDirection = Vector3::crossProduct(this->cameraFront, this->cameraUp);
    leftRightDirection.normalize();
    // Note that (int)bool is either 0 or 1 (false or true, resp.). The result of this operation is thus -1, 0 or 1.
    int upDownMovement = static_cast<int>(this->upActive) - static_cast<int>(this->downActive);
    // Right is the positive direction, left the negative.
    int leftRightMovement = static_cast<int>(this->rightActive) - static_cast<int>(this->leftActive);
    // Update the camera position
    this->cameraPos += upDownMovement * this->cameraFront * cameraSpeed;
    this->cameraPos += leftRightMovement * leftRightDirection * cameraSpeed;
    // Update the lookAtMatrix
    lookAtMatrix = getLookAtMatrix();
}

void ViewMatrix::registerWithGlfwWindow(GlfwWindow &w)
{
    keyCallbackUnregisterFunction = w.registerKeyCallback(
            std::bind(&ViewMatrix::processKeyPress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
            [this]() -> void {
                this->keyCallbackUnregisterFunction = 0;
            });
}

const float* ViewMatrix::data() noexcept
{
    return this->lookAtMatrix.data();
}
