#ifndef PERSPECTIVE_PROJECTION_MATRIX_HPP
#define PERSPECTIVE_PROJECTION_MATRIX_HPP

#include "projectionMatrix.hpp"

class PerspectiveProjectionMatrix : public ProjectionMatrix {
    private:
        float iFov, iAr, iNear, iFar;
        Matrix4 createMatrix(void);
    public:
        PerspectiveProjectionMatrix(const float fov, const float width, const float height, const float near, const float far);
        void setFov(const float fov);
        void setNear(const float near);
        void setFar(const float far);
        const float* data(void) const noexcept;
        void setWindowSize(const float width, const float height);
};

#endif //PERSPECTIVE_PROJECTION_MATRIX_HPP
