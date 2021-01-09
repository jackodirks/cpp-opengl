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

Vector3 ViewMatrix::getCameraFront(const float pitch, const float yaw) const
{
    float x = std::cos(yaw) * std::cos(pitch);
    float y = std::sin(pitch);
    float z = std::sin(yaw) * std::cos(pitch);
    // CameraFront is the rotation of the camera.
    return Vector3::normalize(Vector3(x, y, z));
}

void ViewMatrix::unregisterGlfwWindow(void)
{
    if (keyCallbackUnregisterFunction) {
        keyCallbackUnregisterFunction();
        keyCallbackUnregisterFunction = 0;
    }
    this->glfwWindow = nullptr;
}

Matrix4 ViewMatrix::getLookAtMatrix(const Vector3 &worldUp, const Vector3 &cameraDirection, const Vector3 &cameraPos) const
{
    Vector3 cameraRight = Vector3::normalize(Vector3::crossProduct(worldUp, cameraDirection));
    Vector3 cameraUp = Vector3::normalize(Vector3::crossProduct(cameraDirection, cameraRight));
    Matrix4 axisMat({
        cameraRight.x(),        cameraRight.y(),        cameraRight.z(),        0,
        cameraUp.x(),           cameraUp.y(),           cameraUp.z(),           0,
        cameraDirection.x(),    cameraDirection.y(),    cameraDirection.z(),    0,
        0,                      0,                      0,                      1,
    });
    Matrix4 posMat({
        1,  0,  0,  -cameraPos.x(),
        0,  1,  0,  -cameraPos.y(),
        0,  0,  1,  -cameraPos.z(),
        0,  0,  0,  1,
    });
    return axisMat * posMat;
}

ViewMatrix::ViewMatrix(const float sensitivity, const float moveSpeed) : sensitivity(sensitivity), moveSpeed(moveSpeed), pitchLimit(89.0*M_PI/180.0)
{
    upActive = downActive = leftActive = rightActive = false;
    keyCallbackUnregisterFunction = 0;
    glfwWindow = nullptr;
    mouseCursorInFocus = false;

    prevTime = 0;
    prevXpos = 0;
    prevYpos = 0;
    yaw = (90.0 * M_PI)/180.0; // 90 degrees
    pitch = 0;
    cameraPos = Vector3(0, 0, 3);

    // Update sets the lookAtMatrix.
    update();
}

ViewMatrix::~ViewMatrix(void)
{
    unregisterGlfwWindow();
}

void ViewMatrix::update(void)
{
    // World up is a definition
    const Vector3 worldUp = Vector3(0, 1, 0);

    float currentTime = glfwGetTime();
    float deltaTime = currentTime - prevTime;
    prevTime = currentTime;

    if (glfwWindow != nullptr) {
        const GlfwWindow::CursorPosition mousePos = glfwWindow->getCursorPosition();
        if (mouseCursorInFocus) {
            double xOffset = (mousePos.xpos - prevXpos) * sensitivity;
            double yOffset = (mousePos.ypos - prevYpos) * sensitivity;
            yaw += xOffset;
            pitch += yOffset;
            if (pitch > pitchLimit) {
                pitch = pitchLimit;
            } else if (pitch < -pitchLimit) {
                pitch = -pitchLimit;
            }
        }
        mouseCursorInFocus = true;
        prevXpos = mousePos.xpos;
        prevYpos = mousePos.ypos;
    }
    const Vector3 cameraDirection = getCameraFront(pitch, yaw);
    const float cameraSpeed = moveSpeed * deltaTime;

    Vector3 leftRightDirection = Vector3::normalize(Vector3::crossProduct(cameraDirection, worldUp));
    // Note that (int)bool is either 0 or 1 (false or true, resp.). The result of this operation is thus -1, 0 or 1.
    int upDownMovement = static_cast<int>(this->downActive) - static_cast<int>(this->upActive);
    // Right is the positive direction, left the negative.
    int leftRightMovement = static_cast<int>(this->leftActive) - static_cast<int>(this->rightActive);
    // Update the camera position
    cameraPos += upDownMovement * cameraDirection * cameraSpeed;
    cameraPos += leftRightMovement * leftRightDirection * cameraSpeed;

    // Update the lookAtMatrix
    lookAtMatrix = getLookAtMatrix(worldUp, cameraDirection, cameraPos);
}

void ViewMatrix::registerWithGlfwWindow(GlfwWindow &w)
{
    // We cannot be registered to multiple GlfwWindows. Moreover, if the destruct notifier gets called, we know
    // for sure that the GlfwWindow object will cease to exist soon.
    // Therefore, do a complete cleanup if one destruct notifier gets called.
    keyCallbackUnregisterFunction = w.registerKeyCallback(
            std::bind(&ViewMatrix::processKeyPress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
            [this]() -> void {
                this->keyCallbackUnregisterFunction = 0;
                this->unregisterGlfwWindow();
            });
    glfwWindow = &w;
}

const float* ViewMatrix::data() const noexcept
{
    return this->lookAtMatrix.data();
}
