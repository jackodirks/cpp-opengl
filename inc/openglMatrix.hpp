#ifndef OPENGL_MATRIX_HPP
#define OPENGL_MATRIX_HPP

#include "matrix4.hpp"
#include "vector3.hpp"

class OpenGlMatrix {
    private:
        Vector3 translation;
        Vector3 rotation;
        Vector3 scaling;
        Vector3 lateRotation;
        Matrix4 mat;
        Matrix4 toRotationMatrix(const Vector3 &vec) const;
        Matrix4 toTranslationMatrix(const Vector3 &vec) const;
        Matrix4 toScaleMatrix(const Vector3 &vec) const;
        Matrix4 getCombinedMatrix(void) const;
    public:
        OpenGlMatrix();
        OpenGlMatrix& addTranslate(const float x = 0, const float y = 0, const float z = 0);
        OpenGlMatrix& addRotate(const float x = 0, const float y = 0, const float z = 0);
        OpenGlMatrix& addLateRotate(const float x = 0, const float y = 0, const float z = 0);
        OpenGlMatrix& addScale(const float x = 0, const float y = 0, const float z = 0);
        OpenGlMatrix& setTranslate(const float x = 0, const float y = 0, const float z = 0);
        OpenGlMatrix& setRotate(const float x = 0, const float y = 0, const float z = 0);
        OpenGlMatrix& setScale(const float x = 0, const float y = 0, const float z = 0);
        OpenGlMatrix& setLateRotate(const float x = 0, const float y = 0, const float z = 0);
        const float* data(void) noexcept;
};

#endif //OPENGL_MATRIX_HPP
