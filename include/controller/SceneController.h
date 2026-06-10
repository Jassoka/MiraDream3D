//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_SCENECONTROLLER_H
#define MIRADREAM3D_SCENECONTROLLER_H
#include <QObject>

class RenderController;
class Scene;

class SceneController : public QObject
{
    Q_OBJECT
public:
    explicit SceneController(QObject* parent, RenderController *renderController);
    Scene *getScene() const;
    void loadBlankScene();

public slots:
    void importScene(const std::string &path);

private:
    RenderController *mRenderController;
    Scene *mScene;
};

#endif //MIRADREAM3D_SCENECONTROLLER_H
