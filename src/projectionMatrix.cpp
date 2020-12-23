#include "projectionMatrix.hpp"

ProjectionMatrix::~ProjectionMatrix()
{
    unregister();
}

ProjectionMatrix::ProjectionMatrix()
{}

ProjectionMatrix::ProjectionMatrix(const ProjectionMatrix& other) : mat(other.mat)
{}


ProjectionMatrix& ProjectionMatrix::operator=(const ProjectionMatrix& other)
{
    if (this == &other) return *this;
    unregister();
    this->mat = other.mat;
    return *this;
}

void ProjectionMatrix::registerWindowResizeCallback(GlfwWindow& window)
{
    unregister();
    GlfwWindow::ResizeCallbackCookie resizeCallbackCookie = window.registerResizeCallback(
        [this](int w, int h) {
            this->setWindowSize(static_cast<float>(w), static_cast<float>(h));
        },
        [this]() {
            this->unregisterFunction = 0;
        });
    unregisterFunction = [resizeCallbackCookie, &window]() { window.unregisterResizeCallback(resizeCallbackCookie);};
}

void ProjectionMatrix::unregister(void)
{
    if (unregisterFunction) {
        unregisterFunction();
        unregisterFunction = 0;
    }
}

bool ProjectionMatrix::resizeCallbackRegistered(void)
{
    return static_cast<bool>(unregisterFunction);
}
