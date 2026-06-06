//
// Created by jassoka on 6/2/26.
//

#ifndef MIRADREAM3D_ENGINE_H
#define MIRADREAM3D_ENGINE_H
#include "controller/SceneController.h"
#include "view/mainwindow.h"


class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);
    ~Engine() override;
    void start();

    const Scene *getScene() const
    {
        return mSceneController->getScene();
    }
private:
    MainWindow mMainWindow;
    SceneController *mSceneController = nullptr;

};
#endif //MIRADREAM3D_ENGINE_H
