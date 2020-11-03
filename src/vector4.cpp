#include <cmath>
#include <iostream>

#include "vector4.hpp"

Vector4::Vector4(void)
{
   this->vec = {};
}

Vector4::Vector4(const std::array<float, 4> &vector)
{
    this->vec = vector;
}

Vector4::Vector4(const float x, const float y, const float z, const float w)
{
    this->vec = {x, y, z, w};
}

const float* Vector4::data(void) const noexcept
{
    return this->vec.data();
}

Vector4 Vector4::normalize(void)
{
    std::array<float, 4> newVec;
    float unit = 0;
    for(const float &f : this->vec) {
        unit += f*f;
    }
    unit = std::sqrt(unit);
    for (size_t i = 0; i < newVec.size(); ++i) {
        newVec[i] = this->vec[i] * unit;
    }
    return Vector4(newVec);
}

Vector4 Vector4::operator*(const float f)
{
    std::array<float, 4> vecCopy = this->vec;
    for (size_t i = 0; i < vecCopy.size(); ++i) {
        vecCopy[i] *= f;
    }
    return Vector4(vecCopy);
}

Vector4 Vector4::operator+(const Vector4 &vec4)
{
    std::array<float, 4> vecCopy = this->vec;
    for (size_t i = 0; i < vecCopy.size(); ++i) {
        vecCopy[i] += vec4.vec[i];
    }
    return Vector4(vecCopy);
}

Vector4 Vector4::operator-(const Vector4 &vec4)
{
    std::array<float, 4> vecCopy = this->vec;
    for (size_t i = 0; i < vecCopy.size(); ++i) {
        vecCopy[i] -= vec4.vec[i];
    }
    return Vector4(vecCopy);
}

void Vector4::print(void)
{
    for(const float &f : this->vec) {
        std::cout << f << std::endl;
    }
}
