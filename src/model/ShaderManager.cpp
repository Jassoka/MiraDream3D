//
// Created by jassoka on 6/1/26.
//

#include "model/ShaderManager.h"
#include "util/QTResourceManager.hpp"
#include <string>
#include "util/file_funcs.hpp"


GLuint ShaderManager::getShaderProgram(const std::string& name)
{
    return mShaderPrograms.at(name);
}

void ShaderManager::createProgram(const std::string &name, const std::vector<uint32_t>& shaders)
{
    const GLuint shaderProgram = mGlFuncs->glCreateProgram();
    for (const auto shader : shaders)
        mGlFuncs->glAttachShader(shaderProgram, shader);

    mGlFuncs->glLinkProgram(shaderProgram);

    int success;
    mGlFuncs->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        mGlFuncs->glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    /*
    for (const auto shader : shaders)
        mGlFuncs->glDeleteShader(shader);
    */
    mShaderPrograms.insert(std::make_pair(name, shaderProgram));
}

GLuint ShaderManager::compileShader(const std::string &shaderCode, const GLenum shaderType)
{
    const char *codeShaderC = shaderCode.c_str();

    const GLuint shaderID = mGlFuncs->glCreateShader(shaderType);

    mGlFuncs->glShaderSource(shaderID, 1, &codeShaderC, nullptr);
    mGlFuncs->glCompileShader(shaderID);

    GLint success;
    mGlFuncs->glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[1024];
        mGlFuncs->glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);

        std::cerr << " Erreur de compilation GLSL" << shaderID << std::endl
                  << infoLog << std::endl;

        mGlFuncs->glDeleteShader(shaderID);
        exit(1);
    }
    return shaderID;
}

void ShaderManager::clearShaders()
{
    for (const auto& [name, programID] : mShaderPrograms)
    {
        // Get nb of shaders per program
        GLint shaderCount = 0;
        mGlFuncs->glGetProgramiv(programID, GL_ATTACHED_SHADERS, &shaderCount);

        if (shaderCount > 0) {
            std::vector<GLuint> shaders(shaderCount);
            mGlFuncs->glGetAttachedShaders(programID, shaderCount, nullptr, shaders.data());

            for (const GLuint shaderID : shaders) {
                mGlFuncs->glDetachShader(programID, shaderID);
                mGlFuncs->glDeleteShader(shaderID);
            }
        }
    }
}


GLuint ShaderManager::compileQTRessourceShader(const std::string &virtualPath, const GLenum shaderType)
{
    const std::string code = QTResourceManager::readEmbeddedRessource(virtualPath);
    return compileShader(code, shaderType);
}
