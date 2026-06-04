//
// Created by jassoka on 6/1/26.
//

#include "model/ShaderManager.h"
#include <string>
#include "util/file_funcs.hpp"

void ShaderManager::execShader(std::string& name, QOpenGLFunctions *glFuncs)
{
    const GLuint shaderID = mShaders.at(name);
    glFuncs->glUseProgram(shaderID);
}

void ShaderManager::loadShader(const std::string &shaderCode, const std::string &name, const GLenum shaderType, QOpenGLFunctions *glFuncs)
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

        std::cerr << " Erreur de compilation GLSL" << name << std::endl
                  << infoLog << std::endl;

        glFuncs->glDeleteShader(shaderID);
        exit(1);
    }

    mShaders.insert(make_pair(name, shaderID));
}
