//
// Created by jassoka on 6/1/26.
//

#include "model/Renderer.h"
#include "util/QTResourceManager.hpp"

Renderer::Renderer(const float initialAspectRatio, QOpenGLFunctions *glFuncs):
mEngineCamera(Camera (
        defaultEngineCameraUp,
        defaultEngineCameraRight,
        defaultEngineCameraLookAt,
        defaultEngineCameraPosition,
        defaultEngineCameraFOV,
        defaultEngineCameraNearPlane,
        defaultEngineCameraFarPlane,
        initialAspectRatio
    ))
{
    mGlFuncs = glFuncs;
    mScene = new Scene();
}

Renderer::~Renderer()
{
    delete mScene;
}

void Renderer::initShaders()
{
    std::string const VertexShader = QTResourceManager::readEmbeddedRessource(":/assets/shaders/standard.vert");
    this->mShaderManager.loadShader(VertexShader, "Vertex Shader", GL_VERTEX_SHADER, this->mGlFuncs);
}
