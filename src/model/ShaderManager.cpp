//
// Created by jassoka on 6/1/26.
//

#include "model/ShaderManager.h"
#include <string>
#include "util/file_funcs.hpp"

GLuint ShaderManager::getShaderProgram(const std::string& name) const
{
    return mShaderPrograms.at(name);
}

void ShaderManager::createProgram(const std::string &name, const std::vector<uint32_t>& shaders, QOpenGLFunctions *glFuncs)
{
    const GLuint shaderProgram = glFuncs->glCreateProgram();
    for (const auto shader : shaders)
        glFuncs->glAttachShader(shaderProgram, shader);

    glFuncs->glLinkProgram(shaderProgram);

    int success;
    glFuncs->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glFuncs->glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    /*
    for (const auto shader : shaders)
        glFuncs->glDeleteShader(shader);
    */
    mShaderPrograms.insert(std::make_pair(name, shaderProgram));
}

GLuint ShaderManager::compileShader(const std::string &shaderCode, const GLenum shaderType, QOpenGLFunctions *glFuncs)
{
    const char *codeShaderC = shaderCode.c_str();

    const GLuint shaderID = glFuncs->glCreateShader(shaderType);

    glFuncs->glShaderSource(shaderID, 1, &codeShaderC, nullptr);
    glFuncs->glCompileShader(shaderID);

    GLint success;
    glFuncs->glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[1024];
        glFuncs->glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);

        std::cerr << " Erreur de compilation GLSL" << shaderID << std::endl
                  << infoLog << std::endl;

        glFuncs->glDeleteShader(shaderID);
        exit(1);
    }
    return shaderID;
}
