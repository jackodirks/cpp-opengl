#include "projectionMatrix.hpp"

ProjectionMatrix::~ProjectionMatrix()
{
    unregister();
}

ProjectionMatrix::ProjectionMatrix(const ProjectionMatrix&)
{}

ProjectionMatrix& ProjectionMatrix::operator=(const ProjectionMatrix& other)
{
    if (this == &other) return *this;
    unregister();
    return *this;
}

void ProjectionMatrix::registerWindowResizeCallback(GlfwWindow& window)
{
    unregister();
    unregisterFunction = window.registerResizeCallback(
        [this](int w, int h) {
            this->setWindowSize(static_cast<float>(w), static_cast<float>(h));
        },
        [this]() {
            this->unregisterFunction = 0;
        });
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
