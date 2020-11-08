#ifndef OPENGL_MATRIX_HPP
#define OPENGL_MATRIX_HPP

#include "matrix4.hpp"

class OpenGlMatrix : public Matrix4 {
    public:
        OpenGlMatrix& translate(const float x = 0, const float y = 0, const float z = 0);
        OpenGlMatrix& rotate(const float x = 0, const float y = 0, const float z = 0);
        OpenGlMatrix& scale(const float x = 0, const float y = 0, const float z = 0);
};

#endif //OPENGL_MATRIX_HPP
