#include <cmath>

#include "perspectiveProjectionMatrix.hpp"


Matrix4 PerspectiveProjectionMatrix::createMatrix(void)
{

    return Matrix4({
        1/(this->ar*std::tan(this->fov/2))    , 0.0                          , 0.0                                                       , 0.0,
        0.0                                   , 1/(std::tan(this->fov/2))    , 0.0                                                       , 0.0,
        0.0                                   , 0.0                          , -1*((this->near + this->far) / (this->far - this->near))  , (-2*this->far*this->near)/(this->far - this->near),
        0.0                                   , 0.0                          , -1.0                                                      , 0.0
    });
}

PerspectiveProjectionMatrix::PerspectiveProjectionMatrix(const float fov, const float width, const float height, const float near, const float far)
{
   this->fov = fov;
   this->ar = width/height;
   this->near = near;
   this->far = far;
   this->mat = this->createMatrix();
}

const float* PerspectiveProjectionMatrix::data(void) const noexcept
{
    return this->mat.data();
}

void PerspectiveProjectionMatrix::setFov(const float fov)
{
    this->fov = fov;
    this->mat = this->createMatrix();
}

void PerspectiveProjectionMatrix::setNear(const float near)
{
    this->near = near;
    this->mat = this->createMatrix();
}

void PerspectiveProjectionMatrix::setFar(const float far)
{
    this->far = far;
    this->mat = this->createMatrix();
}

void PerspectiveProjectionMatrix::setWindowSize(float width, float height)
{
    this->ar = width/height;
    this->mat = this->createMatrix();
}
