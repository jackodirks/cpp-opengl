#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include <array>

class Matrix4 {
    private:
        std::array<float, 16> mat;
    public:
        Matrix4(void);
        Matrix4(const std::array<float, 16> &matrix4);
        Matrix4(const Matrix4 &matrix4);
        Matrix4& operator*=(const float f);
        Matrix4& operator*=(const Matrix4 &matrix4);
        Matrix4& operator+=(const Matrix4 &matrix4);
        Matrix4& operator-=(const Matrix4 &matrix4);
        const float* data(void) const noexcept;
};

Matrix4 operator*(Matrix4 lhs, const float rhs);
Matrix4 operator*(Matrix4 lhs, const Matrix4 &rhs);
Matrix4 operator+(Matrix4 lhs, const Matrix4 &rhs);
Matrix4 operator-(Matrix4 lhs, const Matrix4 &rhs);
#endif //MATRIX4_HPP
