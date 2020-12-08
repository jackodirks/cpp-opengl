#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <cerrno>
#include <cstring>

#include "shaderProgram.hpp"

std::string ShaderProgram::getCompilationError(GLuint shader)
{
    GLint logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize == 0)
        return "glGetShaderiv reports GL_INFO_LOG_LENGTH == 0";
    std::string compilerOutput;
    compilerOutput.resize(static_cast<std::string::size_type>(logSize));
    glGetShaderInfoLog(shader, logSize, NULL, &compilerOutput[0]);
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
    glGetProgramInfoLog(shaderProgram, logSize, NULL, &linkerOutput[0]);
    return linkerOutput;
}


ShaderProgram::ShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    // Load and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    try {
        std::ifstream vertexShaderFile(vertexShaderPath.c_str());
        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::stringstream vertexShaderContents;
        vertexShaderContents << vertexShaderFile.rdbuf();
        std::string const &vertexCodeString = vertexShaderContents.str();
        char const *code = vertexCodeString.c_str();
        glShaderSource(vertexShader, 1, &code, NULL);
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
    try {
        std::ifstream fragmentShaderFile(fragmentShaderPath.c_str());
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::stringstream fragmentShaderContents;
        fragmentShaderContents << fragmentShaderFile.rdbuf();
        std::string const &fragmentCodeString = fragmentShaderContents.str();
        char const *code = fragmentCodeString.c_str();
        glShaderSource(fragmentShader, 1, &code, NULL);
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
    if (shaderProgram != 0)
        glDeleteProgram(shaderProgram);
}
