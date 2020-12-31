#include "vector3.hpp"

Vector3::Vector3(const float x, const float y, const float z)
    : Vector4(x, y, z, 0)
{}

Vector3::Vector3(const Vector4& vec) : Vector4(vec)
{}

Vector3 Vector3::crossProduct(const Vector3& lhs, const Vector3& rhs)
{
    float x = (lhs.vec[1] * rhs.vec[2]) - (lhs.vec[2] * rhs.vec[1]);
    float y = (lhs.vec[2] * rhs.vec[0]) - (lhs.vec[0] * rhs.vec[2]);
    float z = (lhs.vec[0] * rhs.vec[1]) - (lhs.vec[1] * rhs.vec[0]);
    return Vector3(x, y, z);
}
