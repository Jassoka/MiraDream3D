//
// Created by jassoka on 6/1/26.
//

#ifndef MIRADREAM3D_SHADER_MANAGER_H
#define MIRADREAM3D_SHADER_MANAGER_H

#include <string>
#include <unordered_map>
#include <QtOpenGL>

struct DefineField;

/** @brief Class for managing GPU shader programs */
class ShaderManager
{
public:
    static void initialize(QOpenGLFunctions* glFuncs) { mGlFuncs = glFuncs; }

    /**
     * @brief Compiles a shader program in GPU memory using a QT Ressource path
     * @param virtualPath Virtual path associated to QT Ressource
     * @param shaderType Type of shader
     * @param defines Optional, Vector of GLSL compile-time definitions
     * @return Shader program index
     */
    static GLuint compileQTRessourceShader(const std::string &virtualPath, GLenum shaderType, const std::vector<DefineField> &defines = {});

    /** @brief Clears all programs known by the current instance of ShaderManager */
    static void clearShaders();
    /**
     * @brief Compiles a program using shaders
     * @param name Name of program
     * @param shaders List of compiled shaders' indices in OpenGL
     */
    static void createProgram(const std::string &name, const std::vector<GLuint>& shaders);
    /**
     * @brief Returns OpenGL index for program
     * @param name Name of program
     */
    static GLuint getShaderProgram(const std::string &name);

private:

    /**
     * @brief Compiles a shader program in GPU memory
     * @param shaderCode String containing the code as a string
     * @param shaderType Type of shader
     * @return Shader program index
     */
    static GLuint compileShader(const std::string &shaderCode, GLenum shaderType);
    inline static std::unordered_map<std::string, uint32_t> mShaderPrograms;
    inline static QOpenGLFunctions *mGlFuncs = nullptr;
};

#endif //MIRADREAM3D_SHADER_MANAGER_H
