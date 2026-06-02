//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_ENGINE_H
#define MIRADREAM3D_ENGINE_H
#include "SceneController.h"


class Engine
{
public:
    const Scene *getScene() const;
private:
    SceneController *mSceneController = nullptr;

};
#endif //MIRADREAM3D_ENGINE_H
