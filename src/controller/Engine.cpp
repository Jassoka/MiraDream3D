//
// Created by jassoka on 6/2/26.
//
#include "controller/Engine.h"


Engine::Engine(QObject *parent) :
QObject(parent),
mSceneController(new SceneController(this))
{
}

Engine::~Engine()
{
    delete mSceneController;
}

void Engine::start()
{
    mMainWindow.setScene(mSceneController->getScene());
    mMainWindow.show();
}
