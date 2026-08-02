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

void RenderController::rotateAroundAnchor(const float dPhi, const float dTheta)
{
    mRenderer->getEngineCamera()->rotateAroundAnchor(dPhi, dTheta);
    changedCamera();
}

void RenderController::cameraStrafe(const float dx, const float dy)
{
    //TODO un peu wanky, bouge très peu si on se rapproche trop du anchor point auquel cas il faudra dezoom
    const float d = std::sqrt(mRenderer->getEngineCamera()->getAnchorPointDistance());
    mRenderer->getEngineCamera()->strafeCamera(-d*dx, -d*dy);
    changedCamera();
}

void RenderController::cameraZoom(const float factor)
{
    Camera *engineCamera = mRenderer->getEngineCamera();
    engineCamera->zoom(factor);

    // adjusting plane based on distance
    const float d = engineCamera->getAnchorPointDistance();
    engineCamera->setFarPlane( std::max(100.0f * d, 100.f)) ;
    engineCamera->setNearPlane( std::max(0.1f * d, 0.001f));

    changedCamera();
}

void RenderController::onSetViewportMode(const ViewportMode mode)
{
    mCurrViewportMode = mode;
    changedGeometry();
}

void RenderController::onToggleGrid()
{
    emit toggledGrid(mRenderer->toggleGrid());
    changedCamera();
}

int32_t RenderController::pickFromScreen(const int x, const int y)
{
    const auto res = mRenderer->readPickingBuffer(x, y);
    if (res >= 0)
        changedCamera();
    return res;
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



