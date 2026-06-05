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
    std::unordered_map<std::string, uint32_t> mShaderPrograms;
public:
    static GLuint compileShader(const std::string &shaderCode, const GLenum shaderType, QOpenGLFunctions *glFuncs);
    void createProgram(const std::string &name, const std::vector<GLuint>& shaders, QOpenGLFunctions *glFuncs);
    GLuint getShaderProgram(const std::string &name) const;
};

#endif //MIRADREAM3D_SHADER_MANAGER_H
