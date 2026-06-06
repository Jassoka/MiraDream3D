//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_SCENECONTROLLER_H
#define MIRADREAM3D_SCENECONTROLLER_H
#include <QObject>

#include "model/Scene.h"

class SceneController : public QObject
{
    Q_OBJECT
public:
    SceneController(QObject* parent);
    const Scene* getScene() const;

    void setScene(const Scene& scene)
    {
        mScene = scene;
    }

private:
    Scene mScene;
};

#endif //MIRADREAM3D_SCENECONTROLLER_H
