#ifndef PROJECTION_MATRIX_HPP
#define PROJECTION_MATRIX_HPP

#include "matrix4.hpp"

class ProjectionMatrix {
    protected:
        Matrix4 mat;
    public:
        virtual const float* data(void) const noexcept = 0;
        virtual void setWindowSize(const float width, const float height) = 0;
};

#endif //PROJECTION_MATRIX_HPP
