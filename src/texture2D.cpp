#include <sstream>
#include <FreeImagePlus.h>
#include <utility>

#include "texture2D.hpp"
#include "glErrorToString.hpp"

Texture2D::Texture2D(const std::string &imagePath, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
{
    // Cannot generate an error.
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    // These can generate an error
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        glDeleteTextures(1, &this->texture);
        std::ostringstream errStream;
        errStream << "Parameter wrapS with value " << wrapS << " is invalid: " << glErrorToString(err);
        throw std::runtime_error(errStream.str());
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        glDeleteTextures(1, &this->texture);
        std::ostringstream errStream;
        errStream << "Parameter wrapT with value " << wrapT << " is invalid: " << glErrorToString(err);
        throw std::runtime_error(errStream.str());
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        glDeleteTextures(1, &this->texture);
        std::ostringstream errStream;
        errStream << "Parameter minFilter with value " << minFilter << " is invalid: " << glErrorToString(err);
        throw std::runtime_error(errStream.str());
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        glDeleteTextures(1, &this->texture);
        std::ostringstream errStream;
        errStream << "Parameter magFilter with value " << minFilter << " is invalid: " << glErrorToString(err);
        throw std::runtime_error(errStream.str());
    }

    fipImage img = fipImage();
    if (!img.load(imagePath.c_str())) {
        glDeleteTextures(1, &this->texture);
        std::ostringstream errStream;
        errStream << "FreeImage failed to load image from path " << imagePath;
        throw std::runtime_error(errStream.str());
    }
    if (!img.convertTo32Bits()) {
        glDeleteTextures(1, &this->texture);
        std::ostringstream errStream;
        errStream << "FreeImage failed to convert image to 32 bit";
        throw std::runtime_error(errStream.str());
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.getWidth(), img.getHeight(),  0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)img.accessPixels());
    err = glGetError();
    if (err != GL_NO_ERROR) {
        glDeleteTextures(1, &this->texture);
        std::ostringstream errStream;
        errStream << "glTexImage2D failed with error " << glErrorToString(err);
        throw std::runtime_error(errStream.str());
    }
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &this->texture);
}

Texture2D::Texture2D(Texture2D&& other) : texture(std::exchange(other.texture, 0))
{}

Texture2D& Texture2D::operator=(Texture2D&& other)
{
    this->texture = std::exchange(other.texture, 0);
    return *this;
}

GLuint Texture2D::getTextureId() const
{
    return texture;
}
