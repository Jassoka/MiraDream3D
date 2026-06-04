//
// Created by jassoka on 6/2/26.
//

#include "controller/SceneController.h"

bool SceneController::loadScene(const std::string &path)
{
    return mAssetImporter.loadAssimpScene(const std::string &path)
}

SceneController::SceneController(): mAssetImporter(&mScene)
{
}

const Scene* SceneController::getScene() const
{
    return &mScene;
}
