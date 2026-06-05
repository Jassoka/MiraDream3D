//
// Created by jassoka on 6/1/26.
//

#include "model/Renderer.h"
#include "util/QTResourceManager.hpp"

Renderer::Renderer():
mEngineCamera(Camera (
        defaultEngineCameraUp,
        defaultEngineCameraRight,
        defaultEngineCameraLookAt,
        defaultEngineCameraPosition,
        defaultEngineCameraFOV,
        defaultEngineCameraNearPlane,
        defaultEngineCameraFarPlane,
        1.0
    ))
{
}

void Renderer::render()
{
    const auto vertices = mScene->getMeshes()[0].getVertices();
    const Vertex *vertices_data = vertices.data(); // Pointeur vers les vertices

    // Buffer de vertices
    mVBO.bind();
    mVBO.allocate(vertices_data,vertices.size() * sizeof(Vertex));

    // On choisit le programme du vertex shader
    const std::string programName = "Standard";
    const GLuint programID = mShaderManager.getShaderProgram(programName);
    mGlFuncs->glUseProgram(programID);

    // Arguments de la caméra
    const int viewMatrix= mGlFuncs->glGetUniformLocation(programID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera.computeViewMatrix()[0][0]);

    glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, nullptr);
}

Renderer::~Renderer()
{
    delete mScene;
}

void Renderer::initShaders()
{
    std::string const VertexShaderCode = QTResourceManager::readEmbeddedRessource(":/assets/shaders/standard.vert");
    GLuint vertexShader = ShaderManager::compileShader(VertexShaderCode, GL_VERTEX_SHADER, this->mGlFuncs);
    std::string const FragmentShaderCode = QTResourceManager::readEmbeddedRessource(":/assets/shaders/standard.frag");
    GLuint fragmentShader = ShaderManager::compileShader(FragmentShaderCode, GL_VERTEX_SHADER, this->mGlFuncs);
    std::vector<GLuint> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);

    std::string const programName = "Standard";
    mShaderManager.createProgram(programName, shaders, this->mGlFuncs);

}

void Renderer::initialize(QOpenGLFunctions* glFuncs)
{
    mGlFuncs = glFuncs;

    if (!mVAO.create()) exit(1);
    if (!mVBO.create()) exit(1);

    mVAO.bind();
    // 0: position du vertex
    mGlFuncs->glEnableVertexAttribArray(0);
    mGlFuncs->glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    mVAO.release();

    initShaders();
}

void Renderer::resize(int width, int height)
{
    if (height == 0) height = 1;
    const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    mEngineCamera.setAspectRatio(aspectRatio);
}
