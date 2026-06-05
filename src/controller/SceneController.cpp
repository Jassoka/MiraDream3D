//
// Created by jassoka on 6/2/26.
//

#include "controller/SceneController.h"
#include "model/PrimitiveMeshes.hpp"

SceneController::SceneController(QObject* parent) : QObject(parent),
    mScene(Scene())
{
    mScene.addMesh(PrimitiveMeshes::getCube());
}

const Scene* SceneController::getScene() const
{
    return &mScene;
}
