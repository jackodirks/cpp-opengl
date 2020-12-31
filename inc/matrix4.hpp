#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include <array>

/**This class represents a 4x4 float matrix. It is stored row-based, meaning that the first element of the second row
 * resides at [4].
 * This class attempts to follow the conventional linear algebra.*/
class Matrix4 {
    private:
        std::array<float, 16> mat;
    public:
        /**Default constructor, initializes to the identity matrix.*/
        Matrix4(void);
        /**Constructor from std::array. Allows intializations like Matrix4 a = {0};*/
        Matrix4(const std::array<float, 16> &matrix4);
        /**Multiply each element of the matrix with f.*/
        Matrix4& operator*=(const float f);
        /**Multiply two matrices, following conventional linear algebra.*/ 
        Matrix4& operator*=(const Matrix4 &other);
        /**Add the elements of the other matrix to this one.*/
        Matrix4& operator+=(const Matrix4 &other);
        /**Subtract the element of the other matrix from this one.*/
        Matrix4& operator-=(const Matrix4 &other);
        /**Return a pointer to a c-style array containing the current data in this matrix, row-based.
         * See also the documentation for std::array::data.*/
        const float* data(void) const noexcept;
};
/**Create a new Matrix4 from a multiplication. See Matrix4::operator*=.*/
Matrix4 operator*(Matrix4 lhs, const float rhs);
/**Create a new Matrix4 from a multiplication. See Matrix4::operator*=.*/
Matrix4 operator*(Matrix4 lhs, const Matrix4 &rhs);
/**Create a new Matrix4 from an addition. See Matrix4::operator+=.*/
Matrix4 operator+(Matrix4 lhs, const Matrix4 &rhs);
/**Create a new Matrix4 from a substraction. See Matrix4::operator-=.*/
Matrix4 operator-(Matrix4 lhs, const Matrix4 &rhs);
#endif //MATRIX4_HPP
