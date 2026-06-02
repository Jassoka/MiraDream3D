//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_SCENECONTROLLER_H
#define MIRADREAM3D_SCENECONTROLLER_H
#include "model/Scene.h"

class SceneController
{
public:
    SceneController();
    const Scene* getScene() const;
private:
    Scene mScene;
};

#endif //MIRADREAM3D_SCENECONTROLLER_H
