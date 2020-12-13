#include "orthographicProjectionMatrix.hpp"

Matrix4 OrthographicProjectionMatrix::createMatrix(void)
{
    return Matrix4({
        2/(right - left), 0,              0,              -(right + left)/(right - left),
        0,                  2/(up-bottom),0,              -(up + bottom)/(up - bottom),
        0,                  0,              -2/(far-near),-(far + near)/(far - near),
        0,                  0,              0,              1,
    });
}

OrthographicProjectionMatrix::OrthographicProjectionMatrix(const float left, const float right, const float bottom, const float up, const float near, const float far)
{
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->up = up;
    this->near = near;
    this->far = far;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setLeft(const float left)
{
    this->left = left;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setRight(const float right)
{
    this->right = right;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setBottom(const float bottom)
{
    this->bottom = bottom;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setUp(const float up)
{
    this->up = up;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setNear(const float near)
{
    this->near = near;
    this->mat = createMatrix();
}

void OrthographicProjectionMatrix::setFar(const float far)
{
    this->far = far;
    this->mat = createMatrix();
}

const float* OrthographicProjectionMatrix::data(void) const noexcept
{
    return this->mat.data();
}

void OrthographicProjectionMatrix::setWindowSize(const float width, const float height)
{
    this->right = width;
    this->up = height;
    this->mat = createMatrix();
}

