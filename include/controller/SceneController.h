//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_SCENECONTROLLER_H
#define MIRADREAM3D_SCENECONTROLLER_H
#include <QObject>

class RenderController;
class Scene;

/**
 * @brief Controller class for managing the main scene
 */
class SceneController : public QObject
{
    Q_OBJECT
public:
    explicit SceneController(QObject* parent, RenderController *renderController);

    Scene *getScene() const;
    /**
     * @brief Empties current scene
     */
    void loadBlankScene() const;

public slots:
    /**
     * @brief Imports a file to the current scene
     */
    void importScene(const std::string &path) const;

private:
    RenderController *mRenderController;
    Scene *mScene;
};

#endif //MIRADREAM3D_SCENECONTROLLER_H
