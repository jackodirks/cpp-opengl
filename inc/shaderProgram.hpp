#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "glad/glad.h"

class ShaderProgram {
    private:
        GLuint shaderProgram;
        std::string getCompilationError(GLuint shader);
        std::string getLinkingError(GLuint shaderProgram);
        GLint getUniformLocation(const std::string &name);

    public:
        ShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
        ~ShaderProgram();
        // Delete the copy constructor and assignment operator, you should not copy this class
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram* operator=(const ShaderProgram&) = delete;
        // We do allow move constructor and move assignment
        ShaderProgram(ShaderProgram&& other);
        ShaderProgram& operator=(ShaderProgram&&);

        void use(void);
        void setUniformMatrix4v(const std::string &name, const size_t count, const bool transpose, const float* value);
};

#endif //SHADER_PROGRAM_HPP
