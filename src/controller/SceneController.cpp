//
// Created by jassoka on 6/2/26.
//

#include "controller/SceneController.h"
#include "model/PrimitiveMeshes.hpp"

SceneController::SceneController(QObject* parent) : QObject(parent),
    mScene(Scene())
{
    mScene.addMesh(PrimitiveMeshes::getCube());
    //TODO: pour l'instant c'est un triangle de merde
/*
    Mesh triangle = Mesh(0);
    triangle.addVertex({-0.5f, -0.5f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0});
    triangle.addVertex({0.5f, -0.5f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0});
    triangle.addVertex({0.0f, 0.5f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0});
    triangle.addFace({0, 1, 2});

    mScene.addMesh(triangle);*/
}

const Scene* SceneController::getScene() const
{
    return &mScene;
}
