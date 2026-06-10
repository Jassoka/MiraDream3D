//
// Created by jassoka on 6/9/26.
//

#include "controller/RenderController.h"
#include "model/Renderer.h"
#include "model/Camera.h"
#include "types.h"

RenderController::RenderController(QObject* parent) :
    QObject(parent), mCurrViewportMode(ViewportMode::SOLID)
{
    mRenderer = new Renderer();
}


RenderController::~RenderController()
{
    delete mRenderer;
}

void RenderController::paint()
{
    if (!hasToRedraw) return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (hasGeometryChanged())
    {
        mRenderer->geometryRedraw(mCurrViewportMode);
        geometryRedrawn();
    }
    else if (hasTopologyChanged())
    {
        //TODO
        topologyRedrawn();
    }
    else if (hasCameraChanged())
    {
        mRenderer->draw(mCurrViewportMode);
        cameraRedrawn();
    }
}


void RenderController::setScene(const Scene *scene) const { mRenderer->setScene(scene); }


void RenderController::onResize(const int width, const int height)
{
    mRenderer->resize(width, height);
    changedCamera();
}

void RenderController::onInitialize(QOpenGLFunctions* glFuncs) const
{
    mRenderer->initialize(glFuncs);
}

void RenderController::onRotateAroundAnchor(const float dPhi, const float dTheta)
{
    mRenderer->getEngineCamera()->rotateAroundAnchor(dPhi, dTheta);
    changedCamera();
}

void RenderController::onCameraStrafe(const float dx, const float dy)
{
    mRenderer->getEngineCamera()->strafeCamera(dx, dy);
    changedCamera();
}

void RenderController::onZoom(const float factor)
{
    mRenderer->getEngineCamera()->zoom(factor);
    changedCamera();
}

void RenderController::onSetViewportMode(const ViewportMode mode)
{
    mCurrViewportMode = mode;
    changedGeometry();
}


