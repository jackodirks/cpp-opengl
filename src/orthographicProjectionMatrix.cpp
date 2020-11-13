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

void OrthographicProjectionMatrix::setLeft(const float left)
{
    this->iLeft = left;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setRight(const float right)
{
    this->iRight = right;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setBottom(const float bottom)
{
    this->iBottom = bottom;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setUp(const float up)
{
    this->iUp = up;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setNear(const float near)
{
    this->iNear = near;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setFar(const float far)
{
    this->iFar = far;
    this->mat = createMatrix();
}

const float* OrthographicProjectionMatrix::data(void) const noexcept
{
    return this->mat.data();
}

void OrthographicProjectionMatrix::setWindowSize(const float width, const float height)
{
    this->iRight = width;
    this->iUp = height;
    this->mat = createMatrix();
}

