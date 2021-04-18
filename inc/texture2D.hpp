#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include "glad/glad.h"

/**This class represents a GL_TEXTURE_2D, generated from an image file.*/
class Texture2D {
    private:
        GLuint texture;
    public:
        /**@brief Constructor
         * @param imagePath A file system path from which the image is loaded. Can be many different formats
         * @param wrapS See GL_TEXTURE_WRAP_S
         * @param wrapT See GL_TEXTURE_WRAP_T
         * @param minFilter See GL_TEXTURE_MIN_FILTER
         * @param magFilter See GL_TEXTURE_MAG_FILTER 
         * @param glInternalFormat The output format. See glTexImage2D internalFormat.*/
        Texture2D(const std::string &imagePath, GLint wrapS = GL_MIRRORED_REPEAT, GLint wrapT = GL_MIRRORED_REPEAT,
                GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR);
        /*@brief Destructor to clean up texture.*/
        ~Texture2D();
        /*Delete the copy operators: We do not want to copy an GL_TEXTURE. Transfer of ownership should happen through move.*/
        Texture2D(const Texture2D&) = delete;
        Texture2D* operator=(const Texture2D&) = delete;
        /*The move constructor/operator.*/
        Texture2D(Texture2D&& other);
        Texture2D& operator=(Texture2D&&);
        /* @brief get the texture id */
        GLuint getTextureId() const;
};

#endif //TEXTURE2D_HPP
