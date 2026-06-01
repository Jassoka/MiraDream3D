//
// Created by jassoka on 6/1/26.
//

#include "model/Renderer.h"

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
    this->mShaderManager.loadShader( PROJECT_PATH "");
}
