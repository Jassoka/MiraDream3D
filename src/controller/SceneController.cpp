//
// Created by jassoka on 6/2/26.
//

#include "controller/SceneController.h"

#include "controller/ErrorController.h"
#include "controller/RenderController.h"
#include "model/Scene.h"
#include "model/AssetImporter.h"
#include "model/PrimitiveMeshes.h"

SceneController::SceneController(QObject* parent, RenderController *renderController) :
QObject(parent), mRenderController(renderController), mScene(new Scene())
{
    const Mesh defaultCube = PrimitiveMeshes::generateCube();
    mScene->addMesh(defaultCube);
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
    try {
        AssetImporter::loadObjFile(path, getScene());
    }
    catch (const std::runtime_error &e ) {
        ErrorController::showError(e);
        loadBlankScene();
    }
    catch (const std::exception &e) {
        exit(3);//erreurs non prevues;
    }

        //TODO faire switch sur extension quand meme
}

void SceneController::loadBlankScene() const
{
    mScene->clearScene();
}
