//
// Created by jassoka on 6/2/26.
//
#include "controller/Engine.h"

Engine::Engine(QObject *parent) :
QObject(parent),mMainWindow(nullptr,this)
{
    mSceneController=new SceneController((QObject*)this);
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
