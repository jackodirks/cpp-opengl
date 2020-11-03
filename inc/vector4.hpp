#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <array>

class Vector4 {
    protected:
        std::array<float, 4> vec;
    public:
        Vector4(void);
        explicit Vector4(const std::array<float, 4> &vector);
        Vector4(const float x, const float y, const float z, const float w);
        const float* data(void) const noexcept;
        Vector4& normalize(void);

        Vector4& operator*=(const float f);
        Vector4& operator+=(const Vector4 &vec4);
        Vector4& operator-=(const Vector4 &vec4);

        void print(void);
};

Vector4 operator*(Vector4 lhs, const float rhs);
Vector4 operator+(Vector4 lhs, const Vector4 &rhs);
Vector4 operator-(Vector4 lhs, const Vector4 &rhs);

#endif //VECTOR4_HPP
