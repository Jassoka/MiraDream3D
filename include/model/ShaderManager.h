//
// Created by jassoka on 6/1/26.
//

#ifndef MIRADREAM3D_SHADER_MANAGER_H
#define MIRADREAM3D_SHADER_MANAGER_H

#include <string>
#include <unordered_map>
#include <QtOpenGL>

class ShaderManager
{
private:
    std::unordered_map<std::string, uint32_t> mShaders;
public:
    void loadShader(const std::string &shaderCode, const std::string &name, GLenum shaderType, QOpenGLFunctions *glFuncs);
    void execShader(std::string &name, QOpenGLFunctions *glFuncs);
};

#endif //MIRADREAM3D_SHADER_MANAGER_H
