#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <array>

/**Represents a 4x1 vector of type float.*/
class Vector4 {
    protected:
        std::array<float, 4> vec;
    public:
        /**Construct a Vector4 from up to four values. Also serves as the default constructor.*/
        Vector4(const float x = 0, const float y = 0, const float z = 0, const float w = 0);
        /**Construct a Vector4 from an array.*/
        Vector4(const std::array<float, 4> &vector);
        /**Return a pointer to a c-style array containing the current data in this vector.
         * See also: std::array::data.*/
        const float* data(void) const noexcept;

        /**@return vec[0]*/
        const float x() const;
        /**@return vec[1]*/
        const float y() const;
        /**@return vec[2]*/
        const float z() const;
        /**@return vec[3]*/
        const float w() const;
        /**@return The value at position i.
         * @warning Throws range_error if i > 3*/
        float operator[](const std::size_t i) const;

        /**Normalize the vector. A normalized vector has a length of 1 (sqrt(a^2 + b^2 + c^2 + ..) = 1).*/
        static Vector4 normalize(Vector4 vec);
        /**Multiply every element of this Vector4 with f.*/
        Vector4& operator*=(const float f);
        /**Add another Vector4 to this one.*/
        Vector4& operator+=(const Vector4 &vec4);
        /**Subtract another Vector4 from this one.*/
        Vector4& operator-=(const Vector4 &vec4);
};
/**Create a new Vector4 from multiplication. See also: Vector4::operator*=.*/
Vector4 operator*(Vector4 lhs, const float rhs);
/**Create a new Vector4 from multiplication. See also: Vector4::operator*=.*/
Vector4 operator*(const float lhs, Vector4 rhs);
/**Create a new Vector4 from addition. See also: Vector4::operator+=.*/
Vector4 operator+(Vector4 lhs, const Vector4 &rhs);
/**Create a new Vector4 from subtraction. See also: Vector4::operator-=.*/
Vector4 operator-(Vector4 lhs, const Vector4 &rhs);

#endif //VECTOR4_HPP
