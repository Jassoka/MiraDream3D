//
// Created by jassoka on 6/1/26.
//

#ifndef MIRADREAM3D_SHADER_MANAGER_H
#define MIRADREAM3D_SHADER_MANAGER_H

#include <string>
#include <unordered_map>
#include <QOpenGLFunctions>
#include <GL/gl.h>
#include "Shader.h"

class ShaderManager
{
private:
    std::unordered_map<std::string, Shader*> mShaders;
public:
    void loadShader(const std::string &shaderCode, const std::string &name, const GLenum shaderType, QOpenGLFunctions *glFuncs);
};

#endif //MIRADREAM3D_SHADER_MANAGER_H
