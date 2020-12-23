#ifndef ORTHOGRAPHIC_PROJECTION_MATRIX_HPP
#define ORTHOGRAPHIC_PROJECTION_MATRIX_HPP

#include "projectionMatrix.hpp"

class OrthographicProjectionMatrix : public ProjectionMatrix {
    private:
        Matrix4 mat;
        float left, right, bottom, up, near, far;
        Matrix4 createMatrix(void);
    public:
        OrthographicProjectionMatrix(const float left, const float right, const float bottom, const float up, const float near, const float far);
        void setLeft(const float left);
        void setRight(const float right);
        void setBottom(const float bottom);
        void setUp(const float up);
        void setNear(const float near);
        void setFar(const float far);
        const float* data(void) const noexcept;
        void setWindowSize(const float width, const float height) override;
};

#endif //ORTHOGRAPHIC_PROJECTION_MATRIX_HPP
