#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

#include "glad/glad.h"
#include "shaderProgram.hpp"

void ShaderProgram::printCompilationError(GLuint shader)
{
    GLint logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize == 0)
        return;
    std::string compilerOutput;
    compilerOutput.resize(static_cast<std::string::size_type>(logSize));
    glGetShaderInfoLog(shader, logSize, NULL, &compilerOutput[0]);
    std::cerr << compilerOutput;
}

void ShaderProgram::printLinkingError(GLuint shaderProgram)
{
    GLint logSize;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize == 0)
        return;
    std::string linkerOutput;
    linkerOutput.resize(static_cast<std::string::size_type>(logSize));

    glGetProgramInfoLog(shaderProgram, logSize, NULL, &linkerOutput[0]);
    std::cerr << linkerOutput;
}


ShaderProgram::ShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    // Load and compile vertex shader
    std::ifstream vertexShaderFile(vertexShaderPath.c_str());
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream vertexShaderContents;
    vertexShaderContents << vertexShaderFile.rdbuf();
    std::string const &codeString = vertexShaderContents.str();
    char const *code = codeString.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &code, NULL);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        printCompilationError(vertexShader);
        glDeleteShader(vertexShader);
        throw std::runtime_error("Vertex shader compilation failed");
    }

    // Load and compile fragment shader
    std::ifstream fragmentShaderFile(fragmentShaderPath.c_str());
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream fragmentShaderContents;
    fragmentShaderContents << fragmentShaderFile.rdbuf();
    code = codeString.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &code, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        printCompilationError(fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        throw std::runtime_error("Fragment shader compilation failed");
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
        printLinkingError(shaderProgram);
        glDeleteProgram(shaderProgram);
        throw std::runtime_error("Shader linking stage failed");
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
