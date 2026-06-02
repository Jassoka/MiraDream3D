//
// Created by jassoka on 6/2/26.
//

#include "controller/SceneController.h"

SceneController::SceneController():
    mScene(Scene())
{
}

const Scene* SceneController::getScene() const
{
    return &mScene;
}
