#ifndef PROJECTION_MATRIX_HPP
#define PROJECTION_MATRIX_HPP

#include <functional>

#include "matrix4.hpp"
#include "glfwWindow.hpp"

/** An abstract class representing a projection matrix, which can be attached to a GlfwWindow.
 * If it is attached (or registerd) it will be notified of screen resolution changes and call setWindowSize in response.
 * @warning When this class is copied (operator= or copy constructor) it will not be attached to any GlfwWindow.
 */
class ProjectionMatrix {
    protected:
        std::function<void(void)> windowSizeUnregisterFunction;
        std::function<void(void)> scrollUnregisterFunction;
        Matrix4 mat;

        /** Unregister from the GlfwWindow callback (if any). This function performs the necessary checks and is thus always safe.
         */
        virtual void unregister(void);
    public:
        /** Default constructor
         */
        ProjectionMatrix() = default;
        /** Copy constructor.
         * @warning This will not register to any GlfwWindow. It thus does nothing.
         */
        ProjectionMatrix(const ProjectionMatrix&);
        /** Destructor. Will unregister from registered GlfwWindow, if any.
         */
        virtual ~ProjectionMatrix();

        /** Copy assigment operator.
         * @warning This will unregister from registered GlfwWindow, if any, and not register to any GlfwWindow.
         */
        virtual ProjectionMatrix& operator=(const ProjectionMatrix& other);

        /** Set window size
         *
         * When a GlfwWindow is registered using registerWindowResizeCallback,
         * this function is called when the object is notified of a screen resolution change
         * @param width The width in pixels.
         * @param height The height in pixels.
         */
        virtual void setWindowSize(const float width, const float height) = 0;
        /**Process scroll input
         *
         * When a GlfwWindow is registered using registerWindowResizeCallback,
         * this function is called when the object is notified of a scroll update.
         * @param xoffset The new x offset.
         * @param yoffset The new y offset.
         */
        virtual void setScrollOffset(const double xoffset, const double yoffset) = 0;
        /**Get a pointer to the projection matrix, row based.
         *
         * This is basically a float[16]. See std::array::data() for more details.
         */
        virtual const float* data(void) const noexcept;
        /** Register with given GlfwWindow.
         * This function will unregister first, if applicable.
         */
        virtual void registerGlfwWindow(GlfwWindow& window);
};

#endif //PROJECTION_MATRIX_HPP
