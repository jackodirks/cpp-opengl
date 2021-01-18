#ifndef ORTHOGRAPHIC_PROJECTION_MATRIX_HPP
#define ORTHOGRAPHIC_PROJECTION_MATRIX_HPP

#include "projectionMatrix.hpp"

/**This class represents a orthographic projection matrix.
 * See https://en.wikipedia.org/wiki/Orthographic_projection.
 *
 * The idea is that an object is either (partially) within the clipping space and shown as is, or not shown at all.
 * An alternative might be the perspective projection, which makes objects wich are farther away smaller.
 * The clipping space is defined by six variables: left, right, bottom, up (or top), near and far.
 *
 * OpenGL displays everything between -1 and 1. Left, bottom and near map to -1. Right, top and far map to 1.
 */
class OrthographicProjectionMatrix : public ProjectionMatrix {
    private:
        float left, right, bottom, up, near, far;
        /**Create an orthographic projection matrix, using the six variables.
         */
        Matrix4 createMatrix(void);
    public:
        /**Default constructor.
         * @param left The left side of the projection matrix. 0 is a good default.
         * @param right The right side of the projection matrix. The width of the window in pixels is a good default.
         * @param bottom The bottom side of the projection matrix. 0 is a good default.
         * @param up The top side of the projection matrix. The height of the window in pixels is a good default.
         * @param near The front/near side of the projection matrix.
         * @param far The back/far side of the projection matrix.
         */
        OrthographicProjectionMatrix(const float left, const float right, const float bottom, const float up, const float near, const float far);
        void setLeft(const float left);
        void setRight(const float right);
        void setBottom(const float bottom);
        void setUp(const float up);
        void setNear(const float near);
        void setFar(const float far);

        /*Update the window size. Left will be set to width, up will be set to height.*/
        void setWindowSize(const float width, const float height) override;
        /*We do not use this function*/
        void setScrollOffset(const double, const double) override {};
};

#endif //ORTHOGRAPHIC_PROJECTION_MATRIX_HPP
