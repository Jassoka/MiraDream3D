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
public:
    static void initialize(QOpenGLFunctions* glFuncs)
    {
        mGlFuncs = glFuncs;
    }
    static GLuint compileQTRessourceShader(const std::string &virtualPath, GLenum shaderType);
    static GLuint compileShader(const std::string &shaderCode, GLenum shaderType);
    /**
     * @brief Clears all programs known by the current instance of ShaderManager
     */
    static void clearShaders();
    static void createProgram(const std::string &name, const std::vector<GLuint>& shaders);
    static GLuint getShaderProgram(const std::string &name);

private:
    inline static std::unordered_map<std::string, uint32_t> mShaderPrograms;
    inline static QOpenGLFunctions *mGlFuncs = nullptr;
};

#endif //MIRADREAM3D_SHADER_MANAGER_H
