//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_ENGINE_H
#define MIRADREAM3D_ENGINE_H
#include <QObject>
class Scene;
class SceneController;
class RenderController;
class MainWindow;

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);
    ~Engine() override;
    void start();

    const Scene *getScene() const;
    SceneController *getSceneController() const;
private:
    MainWindow *mMainWindow;
    SceneController *mSceneController;
    RenderController *mRenderController;

};
#endif //MIRADREAM3D_ENGINE_H
