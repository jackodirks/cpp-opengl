#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "vector4.hpp"

class Vector3 : public Vector4 {
    public:
        Vector3(const Vector4& vec);
        Vector3(const float x = 0, const float y = 0, const float z = 0);
        const float x() const;
        const float y() const;
        const float z() const;

        static Vector3 crossProduct(Vector3& lhs, Vector3& rhs);
};

#endif //VECTOR3_HPP
