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
    VertexShader();
}

Renderer::~Renderer()
{
    delete mScene;
}

void Renderer::VertexShader()
{
    const auto vertices = mScene->getMeshes()[0].getVertices();
    const Vertex *vertices_data = vertices.data(); // Pointeur vers les vertices

    // Buffer de vertices
    mVBO.bind();
    mVBO.allocate(vertices_data,vertices.size() * sizeof(Vertex));

    // On choisit le programme du vertex shader
    const std::string shaderName = "Vertex Shader";
    const GLuint shaderID = mShaderManager.getShaderID(shaderName);
    mGlFuncs->glUseProgram(shaderID);

    // Arguments de la caméra
    const int viewMatrix= mGlFuncs->glGetUniformLocation(shaderID, "viewMatrix");
    mGlFuncs->glUniformMatrix4fv (viewMatrix, 1, GL_FALSE, &mEngineCamera.computeViewMatrix()[0][0]);

    glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, nullptr);
}

void Renderer::initShaders()
{
    std::string const VertexShader = QTResourceManager::readEmbeddedRessource(":/assets/shaders/standard.vert");
    this->mShaderManager.loadShader(VertexShader, "Vertex Shader", GL_VERTEX_SHADER, this->mGlFuncs);
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
