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
        void print(void);
        Matrix4 operator*(const float f);
        Matrix4 operator*(const Matrix4 &matrix4);
        Matrix4 operator+(const Matrix4 &matrix4);
        Matrix4 operator-(const Matrix4 &matrix4);
};

#endif //MATRIX4_HPP
