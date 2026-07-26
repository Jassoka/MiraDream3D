//
// Created by jassoka on 6/9/26.
//

#include "controller/RenderController.h"
#include "model/Renderer.h"
#include "model/Camera.h"
#include "types.h"
#include "view/RenderWidget.h"

RenderController::RenderController(QObject* parent, RenderWidget *render_widget) :
    QObject(parent), mRenderWidget(render_widget), mCurrViewportMode(ViewportMode::SOLID)
{
    connect(mRenderWidget, &RenderWidget::initialize, this, &RenderController::onInitialize, Qt::DirectConnection);
    connect(mRenderWidget, &RenderWidget::paint, this, &RenderController::paint);
    connect(mRenderWidget, &RenderWidget::resize, this, &RenderController::onResize);
    connect(mRenderWidget, &RenderWidget::rotateAroundAnchor, this, &RenderController::onRotateAroundAnchor);
    connect(mRenderWidget, &RenderWidget::strafeCamera, this, &RenderController::onCameraStrafe);

    connect(mRenderWidget, &RenderWidget::zoom, this, &RenderController::onZoom);
    connect(mRenderWidget, &RenderWidget::setViewportMode, this, &RenderController::onSetViewportMode);
    connect(this, &RenderController::callWidgetRedraw, mRenderWidget, &RenderWidget::requestRedraw);
    mRenderer = new Renderer();
}


RenderController::~RenderController()
{
    delete mRenderer;
}

void RenderController::toolChanged(const EditorTool tool)
{
    switch (tool)
    {
    case EditorTool::SELECTION:
        mRenderer->setSelectionMode(SelectionMode::VERTEX);
        break;
    default:
        mRenderer->setSelectionMode(SelectionMode::NONE);
        break;
    }
    changedCamera();
}

void RenderController::paint()
{
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
    else
    {
        mRenderer->draw(mCurrViewportMode);
        cameraRedrawn();
    }
    glFinish();
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

#ifdef TEST_HALFEDGES
void RenderController::onAddHalfEdgeTest(const int i)
{
    mRenderer->addTestHalfEdge(i);
    changedCamera();
}

void RenderController::nextMeshTest()
{
    mRenderer->nextTestMesh();
    changedCamera();
}


void RenderController::nextComponentTest()
{
    mRenderer->nextTestComponent();
    changedCamera();
}

#endif



