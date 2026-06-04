//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_SCENECONTROLLER_H
#define MIRADREAM3D_SCENECONTROLLER_H
#include "controller/AssetImporter.h"
#include "model/Scene.h"

class SceneController
{
public:
    SceneController();
    const Scene* getScene() const;
    bool loadScene(const std::string &path);
private:
    Scene mScene = Scene();
    AssetImporter mAssetImporter;
};

#endif //MIRADREAM3D_SCENECONTROLLER_H
