#ifndef ORTHOGRAPHIC_PROJECTION_MATRIX_HPP
#define ORTHOGRAPHIC_PROJECTION_MATRIX_HPP

#include "projectionMatrix.hpp"

class OrthographicProjectionMatrix : public ProjectionMatrix {
    private:
        float iLeft, iRight, iBottom, iUp, iNear, iFar;
        Matrix4 createMatrix(void);
    public:
        OrthographicProjectionMatrix(const float left, const float right, const float bottom, const float up, const float near, const float far);
        void updateLeft(const float left);
        void updateRight(const float right);
        void updateBottom(const float bottom);
        void updateUp(const float up);
        void updateNear(const float near);
        void updateFar(const float far);
        const float* data(void) const noexcept;
        void updateWindowSize(const float width, const float height);
};

#endif //ORTHOGRAPHIC_PROJECTION_MATRIX_HPP
