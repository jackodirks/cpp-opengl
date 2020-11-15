#include "vector3.hpp"

Vector3::Vector3() : Vector4()
{}

Vector3::Vector3(const float x, const float y, const float z)
    : Vector4(x, y, z, 0)
{}

Vector3 Vector3::crossProduct(const Vector3 &vec3)
Vector3 Vector3::crossProduct(const Vector3 &vec3) const
{
    float x = (this->vec[1] * vec3.vec[2]) - (this->vec[2] * vec3.vec[1]);
    float y = (this->vec[2] * vec3.vec[0]) - (this->vec[0] * vec3.vec[2]);
    float z = (this->vec[0] * vec3.vec[1]) - (this->vec[1] * vec3.vec[0]);
    return Vector3(x, y, z);
}

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
