#include <cmath>

#include "openglMatrix.hpp"

Matrix4 OpenGlMatrix::toRotationMatrix(const Vector3 &vec) const
{
    float x = vec.x();
    float y = vec.y();
    float z = vec.z();
    return Matrix4({
        std::cos(y)*std::cos(z),                        std::cos(y)*std::sin(z),                        -std::sin(y),       0,
        std::sin(x)*std::sin(y)*std::cos(z) - std::cos(x)*std::sin(z), std::sin(x)*std::sin(y)*std::sin(z) + std::cos(x)*std::cos(z), std::sin(x)*std::cos(y), 0,
        std::cos(x)*std::sin(y)*std::cos(z) + std::sin(x)*std::sin(z), std::cos(x)*std::sin(y)*std::sin(z) - std::sin(x)*std::cos(z), std::cos(x)*std::cos(y), 0,
        0,                                    0,                                    0,             1,
    });

}

Matrix4 OpenGlMatrix::toTranslationMatrix(const Vector3 &vec) const
{
    float x = vec.x();
    float y = vec.y();
    float z = vec.z();
    return Matrix4({
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1,
    });
}

Matrix4 OpenGlMatrix::toScaleMatrix(const Vector3 &vec) const
{
    float x = vec.x();
    float y = vec.y();
    float z = vec.z();
    return Matrix4({
       x, 0, 0, 0,
       0, y, 0, 0,
       0, 0, z, 0,
       0, 0, 0, 1,
    });
}

Matrix4 OpenGlMatrix::getCombinedMatrix(void) const
{
    // scale, rotate, translate
    // Default constructor creates identity matrix.
    Matrix4 mat4;
    mat4 *= toTranslationMatrix(this->translation);
    mat4 *= toRotationMatrix(this->rotation);
    mat4 *= toScaleMatrix(this->scaling);
    mat4 *= toRotationMatrix(this->lateRotation);
    return mat4;
}

OpenGlMatrix::OpenGlMatrix()
{
    this->translation = Vector3();
    this->rotation = Vector3();
    this->lateRotation = Vector3();
    this->scaling = Vector3(1, 1, 1);
    this->mat = Matrix4();
}

OpenGlMatrix& OpenGlMatrix::addTranslate(const float x, const float y, const float z)
{
    this->translation += Vector3(x,y,z);
    return *this;
}

OpenGlMatrix& OpenGlMatrix::addRotate(const float x, const float y, const float z)
{
    this->rotation += Vector3(x,y,z);
    return *this;
}

OpenGlMatrix& OpenGlMatrix::addLateRotate(const float x, const float y, const float z)
{
    this->lateRotation += Vector3(x,y,z);
    return *this;
}

OpenGlMatrix& OpenGlMatrix::addScale(const float x, const float y, const float z)
{
    this->scaling += Vector3(x,y,z);
    return *this;
}

OpenGlMatrix& OpenGlMatrix::setTranslate(const float x, const float y, const float z)
{
    this->translation = Vector3(x,y,z);
    return *this;
}

OpenGlMatrix& OpenGlMatrix::setRotate(const float x, const float y, const float z)
{
    this->rotation = Vector3(x,y,z);
    return *this;
}

OpenGlMatrix& OpenGlMatrix::setLateRotate(const float x, const float y, const float z)
{
    this->lateRotation = Vector3(x,y,z);
    return *this;
}

OpenGlMatrix& OpenGlMatrix::setScale(const float x, const float y, const float z)
{
    this->scaling = Vector3(x,y,z);
    return *this;
}

const float* OpenGlMatrix::data() noexcept
{
   this->mat = getCombinedMatrix();
   return this->mat.data();
}
