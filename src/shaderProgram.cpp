#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <cerrno>
#include <cstring>

#include "shaderProgram.hpp"
#include "glErrorToString.hpp"

std::string ShaderProgram::getCompilationError(GLuint shader)
{
    GLint logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize == 0)
        return "glGetShaderiv reports GL_INFO_LOG_LENGTH == 0";
    std::string compilerOutput;
    compilerOutput.resize(static_cast<std::string::size_type>(logSize));
    glGetShaderInfoLog(shader, logSize, nullptr, &compilerOutput[0]);
    return compilerOutput;
}

std::string ShaderProgram::getLinkingError(GLuint shaderProgram)
{
    GLint logSize;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize == 0)
        return "glGetProgramiv reports GL_INFO_LOG_LENGTH == 0";
    std::string linkerOutput;
    linkerOutput.resize(static_cast<std::string::size_type>(logSize));
    glGetProgramInfoLog(shaderProgram, logSize, nullptr, &linkerOutput[0]);
    return linkerOutput;
}

GLint ShaderProgram::getUniformLocation(const std::string &name) const
{
   GLint location = glGetUniformLocation(this->shaderProgram, name.c_str());
   if (location == -1) {
        std::ostringstream errStream;
        errStream << "Retrieving uniform location for uniform with name " << name << " failed: " << glErrorToString(glGetError());
        throw std::runtime_error(errStream.str());
   }
   return location;
}

ShaderProgram::ShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    // Load and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        std::ostringstream errStream;
        errStream << "Creating vertex shader failed: " << glErrorToString(glGetError());
        throw std::runtime_error(errStream.str());
    }
    try {
        std::ifstream vertexShaderFile(vertexShaderPath.c_str());
        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::stringstream vertexShaderContents;
        vertexShaderContents << vertexShaderFile.rdbuf();
        std::string const &vertexCodeString = vertexShaderContents.str();
        char const *code = vertexCodeString.c_str();
        glShaderSource(vertexShader, 1, &code, nullptr);
    } catch (const std::ios_base::failure &e) {
        glDeleteShader(vertexShader);
        // The usage of errno requires POSIX.
        // This trick also has no guarantee of working. It does work, though.
        std::ostringstream errStream;
        errStream << "Failure reading from " << vertexShaderPath << ". Errno: " << errno << " (" << strerror(errno) << ")" << std::endl;
        throw std::runtime_error(errStream.str());
    }
    GLint success;
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        std::string error = getCompilationError(vertexShader);
        glDeleteShader(vertexShader);
        throw std::runtime_error("Vertex shader compilation failed: " + error);
    }

    // Load and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        std::stringstream errStream;
        errStream << "Creating fragment shader failed: " << glErrorToString(glGetError());
        throw std::runtime_error(errStream.str());
    }
    try {
        std::ifstream fragmentShaderFile(fragmentShaderPath.c_str());
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::stringstream fragmentShaderContents;
        fragmentShaderContents << fragmentShaderFile.rdbuf();
        std::string const &fragmentCodeString = fragmentShaderContents.str();
        char const *code = fragmentCodeString.c_str();
        glShaderSource(fragmentShader, 1, &code, nullptr);
    } catch (const std::ios_base::failure &e) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        // The usage of errno requires POSIX.
        std::ostringstream errStream;
        errStream << "Failure reading from " << fragmentShaderPath << ". Errno: " << errno << " (" << strerror(errno) << ")";
        throw std::runtime_error(errStream.str());
    }
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        std::string error = getCompilationError(fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        throw std::runtime_error("Fragment shader compilation failed: " + error);
    }

    // Link the two together
    GLuint &shaderProgram = this->shaderProgram;
    shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        std::stringstream errStream;
        errStream << "Creating shader program failed: " << glErrorToString(glGetError());
        throw std::runtime_error(errStream.str());
    }
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // We no longer need the shaders.
    glDetachShader(shaderProgram, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        std::string error = getLinkingError(shaderProgram);
        glDeleteProgram(shaderProgram);
        throw std::runtime_error("Shader linking stage failed: " + error);
    }
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) : shaderProgram(std::exchange(other.shaderProgram, 0))
{}


ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other)
{
    this->shaderProgram = std::exchange(other.shaderProgram, 0);
    return *this;
}


ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(shaderProgram);
}

void ShaderProgram::use() const
{
    glUseProgram(this->shaderProgram);
}

void ShaderProgram::setUniformMatrix4v(const std::string &name, const size_t count, const bool transpose, const float* value) const
{
    glUniformMatrix4fv(getUniformLocation(name), count, transpose, value);
}

void ShaderProgram::setUniform3f(const std::string &name, const float v0, const float v1, const float v2) const
{
    glUniform3f(getUniformLocation(name), v0, v1, v2);
}


void ShaderProgram::setUniform3f(const std::string &name, const Vector4 &vec) const
{
    glUniform3f(getUniformLocation(name), vec[0], vec[1], vec[2]);
}
