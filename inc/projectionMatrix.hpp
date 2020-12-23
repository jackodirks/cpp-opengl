#ifndef PROJECTION_MATRIX_HPP
#define PROJECTION_MATRIX_HPP

#include <functional>

#include "matrix4.hpp"
#include "glfwWindow.hpp"

class ProjectionMatrix {
    protected:
        std::function<void(void)> unregisterFunction;

        virtual void unregister(void);
    public:
        ProjectionMatrix() = default;
        ProjectionMatrix(const ProjectionMatrix&);
        virtual ~ProjectionMatrix();

        virtual ProjectionMatrix& operator=(const ProjectionMatrix& other);

        virtual void setWindowSize(const float width, const float height) = 0;
        virtual void registerWindowResizeCallback(GlfwWindow& window);
        virtual bool resizeCallbackRegistered(void);
};

#endif //PROJECTION_MATRIX_HPP
