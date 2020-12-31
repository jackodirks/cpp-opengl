#include "vector3.hpp"

Vector3::Vector3(const float x, const float y, const float z)
    : Vector4(x, y, z, 0)
{}

Vector3::Vector3(const Vector4& vec) : Vector4(vec)
{}

const float Vector3::x() const
{
    return this->vec[0];
}

const float Vector3::y() const
{
    return this->vec[1];
}

const float Vector3::z() const
{
    return this->vec[2];
}

Vector3 Vector3::crossProduct(Vector3& lhs, Vector3& rhs)
{
    float x = (lhs.vec[1] * rhs.vec[2]) - (lhs.vec[2] * rhs.vec[1]);
    float y = (lhs.vec[2] * rhs.vec[0]) - (lhs.vec[0] * rhs.vec[2]);
    float z = (lhs.vec[0] * rhs.vec[1]) - (lhs.vec[1] * rhs.vec[0]);
    return Vector3(x, y, z);
}
