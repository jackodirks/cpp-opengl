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

void Matrix4::print(void)
{
    std::cout << std::fixed;
    std::cout << std::setprecision(4);
    for (size_t i = 0; i < 4; ++i) {
        std::cout << "|";
        for (size_t j = 0; j < 3; ++j) {
            std::cout << mat[i * 4 + j] << "\t";
        }
        std::cout << mat[i*4 + 3] << "|" << std::endl;
    }
}

Matrix4 Matrix4::operator*(const float f)
{
    std::array<float, 16> matCopy = this->mat;
    for (size_t i = 0; i < matCopy.size(); ++i) {
        matCopy[i] *= f;
    }
    return Matrix4(matCopy);
}

Matrix4 Matrix4::operator*(const Matrix4 &matrix4)
{
    std::array<float, 16> res = {};
    for (size_t i = 0; i < 4; ++i){
        for (size_t j = 0; j < 4; ++j){
            for (size_t k = 0; k < 4; ++k){
                res[i*4 + j] += this->mat[i*4 + k] * matrix4.mat[k*4 + j];
            }
        }
    }
    return Matrix4(res);
}

Matrix4 Matrix4::operator+(const Matrix4 &matrix4)
{
    std::array<float, 16> matCopy = this->mat;
    for (size_t i = 0; i < matCopy.size(); ++i) {
        matCopy[i] += matrix4.mat[i];
    }
    return Matrix4(matCopy);
}

Matrix4 Matrix4::operator-(const Matrix4 &matrix4)
{
    std::array<float, 16> matCopy = this->mat;
    for (size_t i = 0; i < matCopy.size(); ++i) {
        matCopy[i] -= matrix4.mat[i];
    }
    return Matrix4(matCopy);
}


const float* Matrix4::data(void) const noexcept
{
    return this->mat.data();
}
