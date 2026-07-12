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
    loadBlankScene();
    mRenderController->changedGeometry();
    try {
        SceneImport importData;
        std::string warnings;
        AssetImporter::importFile(OBJ, path, importData, warnings);
        appendImport(importData);
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

void SceneController::appendImport(SceneImport& import)
{
    auto &importMeshes = import.mLocalMeshes;
    auto &sceneMeshes = mScene.mMeshList;

    std::ostringstream warnings;

    // Generating maps
    std::unordered_map<uint32_t, uint32_t> localToGlobalTextureMap;
    std::unordered_map<uint32_t, uint32_t> localToGlobalMaterialMap;
    for (const auto &[path, localId] : import.mTexturePaths)
    {
        const int32_t globalId = mScene.getTextureId(path);
        if (globalId < 0)
        {
            warnings << "file not found: " << path << '\n';
            localToGlobalTextureMap[localId] = DEFAULT_TEXTURE;
        }
        else
            localToGlobalTextureMap[localId] = globalId;
    }

    for (const auto &[name, localId] : import.mLocalMaterialRegistry.materialNames)
    {
        Material &material = import.mLocalMaterialRegistry.getMaterial(localId);
        localToGlobalMaterialMap[localId] = mScene.createNewMaterial(name, material);
        // remapping texture to correct index
        auto globalId = localToGlobalMaterialMap[localId];
        mScene.mMaterialRegistry.getMaterial(globalId).ColorTextureID = localToGlobalTextureMap[material.ColorTextureID];
    }

    for (auto &mesh: importMeshes)
    {
        auto &meshMaterials = mesh.getMaterialList();
        for (int i = 0; i < meshMaterials.size(); i++) { meshMaterials[i] = localToGlobalMaterialMap[meshMaterials[i]]; }
    }

    sceneMeshes.reserve(sceneMeshes.size()+importMeshes.size());
    sceneMeshes.insert(sceneMeshes.end(), importMeshes.begin(), importMeshes.end());

    const auto warningsStr = warnings.str();
    if (!warningsStr.empty())
        ErrorController::showWarnings(warningsStr);
}
