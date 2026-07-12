//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_SCENECONTROLLER_H
#define MIRADREAM3D_SCENECONTROLLER_H
#include <QObject>

#include "model/Scene.h"

class SceneImport;
class RenderController;

/**
 * @brief Controller class for managing the main scene
 */
class SceneController : public QObject
{
    Q_OBJECT
public:
    explicit SceneController(QObject* parent, RenderController *renderController);

    Scene *getScene();
    /**
     * @brief Empties current scene
     */
    void loadBlankScene();

public slots:
    /**
     * @brief Imports a file to the current scene
     */
    void importScene(const std::string &path);

private:
    void appendImport(SceneImport &import);
    RenderController *mRenderController;
    Scene mScene;
};

#endif //MIRADREAM3D_SCENECONTROLLER_H
