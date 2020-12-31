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

Vector4& Vector4::normalize(void)
{
    float unit = 0;
    for(const float &f : this->vec) {
        unit += f*f;
    }
    unit = std::sqrt(unit);
    for (size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] *= unit;
    }
    return *this;
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

Vector4 operator*(Vector4 lhs, const float rhs)
{
    lhs *= rhs;
    return lhs;
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
