//
// Created by jassoka on 6/2/26.
//

#include "controller/SceneController.h"

#include <sstream>

#include "controller/ErrorController.h"
#include "controller/RenderController.h"
#include "model/Scene.h"
#include "../../include/model/importer/AssetImporter.h"
#include "model/PrimitiveMeshes.h"
#include "model/SceneImport.h"

SceneController::SceneController(QObject* parent, RenderController *renderController) :
QObject(parent), mRenderController(renderController), mScene(Scene())
{
    const Mesh defaultCube = PrimitiveMeshes::generateCube();
    mScene.addMesh(defaultCube); //TODO mettre dans un node
    mRenderController->changedGeometry();
}

Scene *SceneController::getScene()
{
    return &mScene;
}

void SceneController::importScene(const std::string &path)
{
    //loadBlankScene();
    mRenderController->changedGeometry();
    try {
        SceneImport importData;
        std::string warnings;
        AssetImporter::importFile(OBJ, path, importData, warnings);
        mScene.appendImport(importData);
        if (!warnings.empty())
            ErrorController::showWarnings(warnings);
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

void SceneController::loadBlankScene()
{
    mScene.clearScene();
}

