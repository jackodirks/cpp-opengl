#include <iostream>
#include <iomanip>

#include "matrix4.hpp"

Matrix4::Matrix4(void)
{
    this->mat = { 1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1};
}

Matrix4::Matrix4(const std::array<float, 16> &matrix4)
{
    this->mat = matrix4;
}


Matrix4::Matrix4(const Matrix4 &matrix4)
{
    this->mat = matrix4.mat;
}

Matrix4& Matrix4::operator*=(const float f)
{
    for (size_t i = 0; i < this->mat.size(); ++i) {
        this->mat[i] *= f;
    }
    return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4 &matrix4)
{
    std::array<float, 16> res = {};
    for (size_t i = 0; i < 4; ++i){
        for (size_t j = 0; j < 4; ++j){
            for (size_t k = 0; k < 4; ++k){
                res[i*4 + j] += this->mat[i*4 + k] * matrix4.mat[k*4 + j];
            }
        }
    }
    this->mat = res;
    return *this;
}

Matrix4 &Matrix4::operator+=(const Matrix4 &matrix4)
{
    for (size_t i = 0; i < this->mat.size(); ++i) {
        this->mat[i] += matrix4.mat[i];
    }
    return *this;
}

Matrix4 &Matrix4::operator-=(const Matrix4 &matrix4)
{
    for (size_t i = 0; i < this->mat.size(); ++i) {
        this->mat[i] -= matrix4.mat[i];
    }
    return *this;
}


const float* Matrix4::data(void) const noexcept
{
    return this->mat.data();
}

Matrix4 operator*(Matrix4 lhs, const float rhs)
{
    lhs *= rhs;
    return lhs;
}

Matrix4 operator*(Matrix4 lhs, const Matrix4 &rhs)
{
    lhs *= rhs;
    return lhs;
}

Matrix4 operator+(Matrix4 lhs, const Matrix4 &rhs)
{
    lhs += rhs;
    return lhs;
}

Matrix4 operator-(Matrix4 lhs, const Matrix4 &rhs)
{
    lhs -= rhs;
    return lhs;
}
