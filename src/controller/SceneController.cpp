//
// Created by jassoka on 6/2/26.
//

#include "controller/SceneController.h"
#include "controller/RenderController.h"
#include "model/Scene.h"
#include "model/AssetImporter.h"
#include "model/PrimitiveMeshes.h"

SceneController::SceneController(QObject* parent, RenderController *renderController) :
QObject(parent), mRenderController(renderController), mScene(new Scene())
{
    //mScene->addMesh(PrimitiveMeshes::getCube());
    mRenderController->changedGeometry();
}

Scene *SceneController::getScene() const
{
    return mScene;
}

void SceneController::importScene(const std::string &path) const
{
    loadBlankScene();
    mRenderController->changedGeometry();
    if (!AssetImporter::loadObjFile(path, getScene())) {
        loadBlankScene();
    }
        //TODO faire switch sur extension quand meme
}

void SceneController::loadBlankScene() const
{
    mScene->clearScene();
}
