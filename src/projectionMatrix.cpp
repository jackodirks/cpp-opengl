#include "projectionMatrix.hpp"

ProjectionMatrix::~ProjectionMatrix()
{
    unregister();
}

ProjectionMatrix::ProjectionMatrix(const ProjectionMatrix&) : windowSizeUnregisterFunction(0), scrollUnregisterFunction(0)
{}

ProjectionMatrix& ProjectionMatrix::operator=(const ProjectionMatrix& other)
{
    if (this == &other) return *this;
    unregister();
    return *this;
}

void ProjectionMatrix::registerGlfwWindow(GlfwWindow& window)
{
    unregister();
    windowSizeUnregisterFunction = window.registerResizeCallback(
        [this](int w, int h) {
            this->setWindowSize(static_cast<float>(w), static_cast<float>(h));
        },
        [this]() {
            this->windowSizeUnregisterFunction = 0;
        });
    scrollUnregisterFunction = window.registerScrollCallback(
        std::bind(&ProjectionMatrix::setScrollOffset, this, std::placeholders::_1, std::placeholders::_2),
        [this]() {
            this->scrollUnregisterFunction = 0;
        });

}

void ProjectionMatrix::unregister(void)
{
    if (windowSizeUnregisterFunction) {
        windowSizeUnregisterFunction();
        windowSizeUnregisterFunction = 0;
    }
    if (scrollUnregisterFunction) {
        scrollUnregisterFunction();
        scrollUnregisterFunction = 0;
    }
}

const float* ProjectionMatrix::data(void) const noexcept
{
    return this->mat.data();
}
