#ifndef PROJECTION_MATRIX_HPP
#define PROJECTION_MATRIX_HPP

#include <functional>

#include "matrix4.hpp"
#include "glfwWindow.hpp"

class ProjectionMatrix {
    protected:
        Matrix4 mat;
        std::function<void(void)> unregisterFunction;

        virtual void unregister(void);
    public:
        ProjectionMatrix();
        ProjectionMatrix(const ProjectionMatrix& other);
        virtual ~ProjectionMatrix();

        ProjectionMatrix& operator=(const ProjectionMatrix& other);

        virtual void setWindowSize(const float width, const float height) = 0;
        virtual void registerWindowResizeCallback(GlfwWindow& window);
};

#endif //PROJECTION_MATRIX_HPP
