#include <cmath>

#include "perspectiveProjectionMatrix.hpp"


Matrix4 PerspectiveProjectionMatrix::createMatrix(void)
{

    return Matrix4({
        1/(this->iAr*std::tan(this->iFov/2))    , 0.0                           , 0.0                                                           , 0.0,
        0.0                                     , 1/(std::tan(this->iFov/2))    , 0.0                                                           , 0.0,
        0.0                                     , 0.0                           , -1*((this->iNear + this->iFar) / (this->iFar - this->iNear))  , (-2*this->iFar*this->iNear)/(this->iFar - this->iNear),
        0.0                                     , 0.0                           , -1.0                                                          , 0.0
    });
}

PerspectiveProjectionMatrix::PerspectiveProjectionMatrix(const float fov, const float width, const float height, const float near, const float far)
{
   this->iFov = fov;
   this->iAr = width/height;
   this->iNear = near;
   this->iFar = far;
   this->mat = this->createMatrix();
}

const float* PerspectiveProjectionMatrix::data(void) const noexcept
{
    return this->mat.data();
}

void PerspectiveProjectionMatrix::updateFov(const float fov)
{
    this->iFov = fov;
    this->mat = this->createMatrix();
}

void PerspectiveProjectionMatrix::updateNear(const float near)
{
    this->iNear = near;
    this->mat = this->createMatrix();
}

void PerspectiveProjectionMatrix::updateFar(const float far)
{
    this->iFar = far;
    this->mat = this->createMatrix();
}

void PerspectiveProjectionMatrix::updateWindowSize(float width, float height)
{
    this->iAr = width/height;
    this->mat = this->createMatrix();
}
