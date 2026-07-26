//
// Created by jassoka on 6/2/26.
//
#include "controller/Engine.h"

#include "controller/EditorController.h"
#include "controller/RenderController.h"
#include "controller/SceneController.h"
#include "view/MainWindow.h"
#include "view/RenderWidget.h"

Engine::Engine(QObject *parent) :
QObject(parent)
{
    mMainWindow = new MainWindow(nullptr);
    RenderWidget *renderWidget = mMainWindow->getRenderWidget();

    mRenderController = new RenderController(static_cast<QObject*>(this), renderWidget);
    mSceneController = new SceneController(static_cast<QObject*>(this), mRenderController);
    mEditorController = new EditorController(static_cast<QObject*>(this), mRenderController);
    mRenderController->setScene(mSceneController->getScene());

    connect(mMainWindow, &MainWindow::importSceneRequested, mSceneController, &SceneController::importScene);


    connect(mMainWindow, &MainWindow::changedEditorTool, mEditorController, &EditorController::setTool);



#ifdef TEST_HALFEDGES
    connect(renderWidget, &RenderWidget::addTestHalfEdgeSignal, mRenderController, &RenderController::onAddHalfEdgeTest);
    connect(renderWidget, &RenderWidget::nextTestMeshSignal, mRenderController, &RenderController::nextMeshTest);
    connect(renderWidget, &RenderWidget::nextTestComponentSignal, mRenderController, &RenderController::nextComponentTest);
#endif

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
