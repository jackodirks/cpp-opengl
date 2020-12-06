#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

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
    std::ifstream vertexShaderFile(vertexShaderPath.c_str());
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream vertexShaderContents;
    vertexShaderContents << vertexShaderFile.rdbuf();
    std::string const &vertexCodeString = vertexShaderContents.str();
    char const *code = vertexCodeString.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &code, NULL);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        std::string error = getCompilationError(vertexShader);
        glDeleteShader(vertexShader);
        throw std::runtime_error("Vertex shader compilation failed: " + error);
    }

    // Load and compile fragment shader
    std::ifstream fragmentShaderFile(fragmentShaderPath.c_str());
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream fragmentShaderContents;
    fragmentShaderContents << fragmentShaderFile.rdbuf();
    std::string const &fragmentCodeString = fragmentShaderContents.str();
    code = fragmentCodeString.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &code, NULL);
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
