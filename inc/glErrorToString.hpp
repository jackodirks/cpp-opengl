#ifndef GL_ERROR_TO_STRING_H
#define GL_ERROR_TO_STRING_H

/**@brief A helper function to translate GLenums representing erros to strings
 * @return The name of the macro as a NUL-terminated string (GL_NO_ERROR, GL_INVALID_ENUM, etcetera) or UNKOWN ERROR.
 */
inline const char* glErrorToString(const GLenum err)
{
    switch (err) {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default:
        return "UNKOWN ERROR";
    }
}

#endif //GL_ERROR_TO_STRING_H
