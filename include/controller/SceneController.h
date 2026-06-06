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
    explicit SceneController(QObject* parent);

    Scene *getScene() ;

    void setScene(const Scene& scene)
    {
        mScene = scene;
    }
    void loadBlankScene();

private:
    Scene mScene;
};

#endif //MIRADREAM3D_SCENECONTROLLER_H
