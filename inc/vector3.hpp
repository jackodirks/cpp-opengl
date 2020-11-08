#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "vector4.hpp"

class Vector3 : public Vector4 {
    public:
        Vector3();
        Vector3(const float x, const float y, const float z); 
        Vector3 crossProduct(const Vector3 &vec3);
        const float x() const;
        const float y() const;
        const float z() const;
};

#endif //VECTOR3_HPP
