//
// Created by jassoka on 6/2/26.
//
#include "controller/Engine.h"

#include "controller/RenderController.h"
#include "controller/SceneController.h"
#include "view/MainWindow.h"
#include "view/RenderWidget.h"

Engine::Engine(QObject *parent) :
QObject(parent)
{
    mMainWindow = new MainWindow(nullptr);

    mRenderController = new RenderController(static_cast<QObject*>(this));
    mSceneController = new SceneController(static_cast<QObject*>(this), mRenderController);
    mRenderController->setScene(mSceneController->getScene());

    connect(mMainWindow, &MainWindow::importSceneRequested, mSceneController, &SceneController::importScene);

    const RenderWidget *renderWidget = mMainWindow->getRenderWidget();
    connect(renderWidget, &RenderWidget::initialize, mRenderController, &RenderController::onInitialize, Qt::DirectConnection);
    connect(renderWidget, &RenderWidget::paint, mRenderController, &RenderController::paint);
    connect(renderWidget, &RenderWidget::resize, mRenderController, &RenderController::onResize);
    connect(renderWidget, &RenderWidget::rotateAroundAnchor, mRenderController, &RenderController::onRotateAroundAnchor);
    connect(renderWidget, &RenderWidget::strafeCamera, mRenderController, &RenderController::onCameraStrafe);

    connect(renderWidget, &RenderWidget::zoom, mRenderController, &RenderController::onZoom);
    connect(renderWidget, &RenderWidget::setViewportMode, mRenderController, &RenderController::onSetViewportMode);
}

Engine::~Engine()
{
    delete mMainWindow;
}

void Engine::start()
{
    mMainWindow->show();
}

const Scene* Engine::getScene() const
{
    return mSceneController->getScene();
}

SceneController* Engine::getSceneController() const
{
    return(mSceneController);
}
