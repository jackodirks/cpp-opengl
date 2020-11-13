#include "orthographicProjectionMatrix.hpp"

Matrix4 OrthographicProjectionMatrix::createMatrix(void)
{
    return Matrix4({
        2/(iRight - iLeft), 0,              0,              -(iRight + iLeft)/(iRight - iLeft),
        0,                  2/(iUp-iBottom),0,              -(iUp + iBottom)/(iUp - iBottom),
        0,                  0,              -2/(iFar-iNear),-(iFar + iNear)/(iFar - iNear),
        0,                  0,              0,              1,
    });
}

OrthographicProjectionMatrix::OrthographicProjectionMatrix(const float left, const float right, const float bottom, const float up, const float near, const float far)
{
    this->iLeft = left;
    this->iRight = right;
    this->iBottom = bottom;
    this->iUp = up;
    this->iNear = near;
    this->iFar = far;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::updateLeft(const float left)
{
    this->iLeft = left;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::updateRight(const float right)
{
    this->iRight = right;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::updateBottom(const float bottom)
{
    this->iBottom = bottom;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::updateUp(const float up)
{
    this->iUp = up;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::updateNear(const float near)
{
    this->iNear = near;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::updateFar(const float far)
{
    this->iFar = far;
    this->mat = createMatrix();
}

const float* OrthographicProjectionMatrix::data(void) const noexcept
{
    return this->mat.data();
}

void OrthographicProjectionMatrix::updateWindowSize(const float width, const float height)
{
    this->iRight = width;
    this->iUp = height;
    this->mat = createMatrix();
}

