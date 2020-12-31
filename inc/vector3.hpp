#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "vector4.hpp"
/**Represents a 3x1 vector with type float. Inherits almost everything from Vector4, the fourth element (w) has an undefined value and should not be used.*/
class Vector3 : public Vector4 {
    public:
        /**Construct a Vector3 from individual elements. Doubles as default constructor*/
        Vector3(const float x = 0, const float y = 0, const float z = 0);
        /**Construct a Vector3 from a Vector4*/
        Vector3(const Vector4& vec);

        /**Calculate the cross product lhs x rhs.*/
        static Vector3 crossProduct(const Vector3& lhs, const Vector3& rhs);
};

#endif //VECTOR3_HPP
