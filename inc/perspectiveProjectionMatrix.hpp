#ifndef PERSPECTIVE_PROJECTION_MATRIX_HPP
#define PERSPECTIVE_PROJECTION_MATRIX_HPP

#include "projectionMatrix.hpp"

/**This class represents a perspective projection. See http://ogldev.atspace.co.uk/www/tutorial12/tutorial12.html.
 *
 * The perspective projection is given by 4 variables: the field-of-view, the aspect ratio, near and far.
 * The field-of-view is the angle of the camera through which we view the world. The aspect ratio is the ratio between
 * the width and the height of the rectrangular area through which we look at the world.
 * Near and far represent how close or far away things are still visible.
 *
 * The main gimmick of the perspective projection is that things that are farther away become smaller.
 * This represents a realistic world view through the human eye.
 */
class PerspectiveProjectionMatrix : public ProjectionMatrix {
    private:
        Matrix4 mat;
        float fov, ar, near, far;
        /**Create an orthographic projection matrix, using the four variables.
         */
        Matrix4 createMatrix(void);
    public:
        /**Constructor.
         * @param fov The field-of-view.
         * @param width Will form aspect ratio (ar) together with height.
         * @param height Will form aspect ratio (ar) together with width.
         * @param near Everything with a z coordinate smaller than this will be clipped.
         * @param far Everything with a z coordinate bigger than this will be clipped.
         */
        PerspectiveProjectionMatrix(const float fov, const float width, const float height, const float near, const float far);
        void setFov(const float fov);
        void setNear(const float near);
        void setFar(const float far);

        /**Get a pointer to the projection matrix, row based.
         *
         * This is basically a float[16]. See std::array::data() for more details.
         */
        const float* data(void) const noexcept;
        /*Update the window size. This will set ar = width/height.
         */
        void setWindowSize(const float width, const float height) override;
};

#endif //PERSPECTIVE_PROJECTION_MATRIX_HPP
