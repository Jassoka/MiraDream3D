//
// Created by jassoka on 6/2/26.
//

#include "controller/SceneController.h"
#include "model/AssetImporter.h"
#include "model/PrimitiveMeshes.hpp"

SceneController::SceneController(QObject* parent) : QObject(parent)
{

    mScene.addMesh(PrimitiveMeshes::getCube());

/*
    Mesh triangle = Mesh(0);
    triangle.addVertex({-0.5f, -0.5f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0});
    triangle.addVertex({0.5f, -0.5f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0});
    triangle.addVertex({0.0f, 0.5f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0});
    triangle.addFace({0, 1, 2});

    mScene.addMesh(triangle);*/
}

Scene *SceneController::getScene()
{
    return &mScene;
}

void SceneController::loadBlankScene() {
    auto s = new Scene();
    mScene=*s;
}
