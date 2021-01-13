#include <cmath>
#include <iostream>

#include "vector4.hpp"

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

const float Vector4::x() const
{
    return vec[0];
}

const float Vector4::y() const
{
    return vec[1];
}

const float Vector4::z() const
{
    return vec[2];
}

const float Vector4::w() const
{
    return vec[3];
}

Vector4 Vector4::normalize(Vector4 vec)
{
    float unit = 0;
    for(const float &f : vec.vec) {
        unit += f*f;
    }
    unit = std::sqrt(unit);
    for (size_t i = 0; i < 4; ++i) {
        vec.vec[i] /= unit;
    }
    return vec;
}

Vector4& Vector4::operator*=(const float f)
{
    for (size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] *= f;
    }
    return *this;
}

Vector4& Vector4::operator+=(const Vector4 &vec4)
{
    for (size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] += vec4.vec[i];
    }
    return *this;
}

Vector4& Vector4::operator-=(const Vector4 &vec4)
{
    for (size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] -= vec4.vec[i];
    }
    return *this;
}

float Vector4::operator[](const std::size_t i) const
{
    if (i > 3)
        throw std::range_error("Vector4 has only 4 elements");
    return this->vec[i];
}

Vector4 operator*(Vector4 lhs, const float rhs)
{
    lhs *= rhs;
    return lhs;
}

Vector4 operator*(const float lhs, Vector4 rhs)
{
    rhs *= lhs;
    return rhs;
}

Vector4 operator+(Vector4 lhs, const Vector4 &rhs)
{
    lhs += rhs;
    return lhs;
}

Vector4 operator-(Vector4 lhs, const Vector4 &rhs)
{
    lhs -= rhs;
    return lhs;
}

Vector4 operator*(const Matrix4 &lhs, const Vector4 &rhs)
{
    return Vector4(
            lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3] * rhs[3],
            lhs[4] * rhs[0] + lhs[5] * rhs[1] + lhs[6] * rhs[2] + lhs[7] * rhs[3],
            lhs[8] * rhs[0] + lhs[9] * rhs[1] + lhs[10] * rhs[2] + lhs[11] * rhs[3],
            lhs[12] * rhs[0] + lhs[13] * rhs[1] + lhs[14] * rhs[2] + lhs[15] * rhs[3]
            );
}
