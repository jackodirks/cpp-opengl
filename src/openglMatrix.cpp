#include <cmath>

#include "openglMatrix.hpp"

OpenGlMatrix& OpenGlMatrix::translate(const float x, const float y, const float z)
{
    Matrix4 transMat({
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1,
    });
    *this *= transMat;
    return *this;
}


OpenGlMatrix& OpenGlMatrix::rotate(const float x, const float y, const float z)
{
    Matrix4 rotMat({
        std::cos(y)*std::cos(z),                        std::cos(y)*std::sin(z),                        -std::sin(y),       0,
        std::sin(x)*std::sin(y)*std::cos(z) - std::cos(x)*std::sin(z), std::sin(x)*std::sin(y)*std::sin(z) + std::cos(x)*std::cos(z), std::sin(x)*std::cos(y), 0,
        std::cos(x)*std::sin(y)*std::cos(z) + std::sin(x)*std::sin(z), std::cos(x)*std::sin(y)*std::sin(z) - std::sin(x)*std::cos(z), std::cos(x)*std::cos(y), 0,
        0,                                    0,                                    0,             1,
    });
    *this *= rotMat;
    return *this;
}

OpenGlMatrix& OpenGlMatrix::scale(const float x, const float y, const float z)
{
    Matrix4 scaleMatrix({
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1,
    });
    *this *= scaleMatrix;
    return *this;
}
