#ifndef PERSPECTIVE_PROJECTION_MATRIX_HPP
#define PERSPECTIVE_PROJECTION_MATRIX_HPP

#include "projectionMatrix.hpp"

class PerspectiveProjectionMatrix : public ProjectionMatrix {
    private:
        float iFov, iAr, iNear, iFar;
        Matrix4 createMatrix(void);
    public:
        PerspectiveProjectionMatrix(const float fov, const float width, const float height, const float near, const float far);
        void updateFov(const float fov);
        void updateNear(const float near);
        void updateFar(const float far);
        const float* data(void) const noexcept;
        void updateWindowSize(const float width, const float height);
};

#endif //PERSPECTIVE_PROJECTION_MATRIX_HPP
