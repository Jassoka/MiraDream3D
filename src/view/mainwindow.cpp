#include "view/mainwindow.h"

#include <iostream>

#include "view/RenderWidget.h"
#include <QOpenGLFunctions>
#include "controller/Engine.h"
#include "RenderDocHelper.hpp"
#include <QFileDialog>
#include <QToolBar>

#include "model/AssetImporter.h"
extern uint32_t frame;
MainWindow::MainWindow(QWidget *parent,Engine* engine)
    : QMainWindow(parent),
    mEngine(engine)
    {
    setWindowTitle(QString::fromUtf8("MiraDream3D"));
    int fps=60;//TODO enlever cette merde hardcodee

    mImportButton = new QPushButton("&Import",this);
    QToolBar* toolbar = addToolBar("Outils");
    toolbar->addWidget(mImportButton);



    mRenderWidget = new RenderWidget(fps,this);
    setCentralWidget(mRenderWidget);




    // When OpenGL funcs are ready, give scene to the RenderWidget
    mRenderWidget->show();
    mImportButton->show();


    connect(mImportButton, &QPushButton::clicked, this, [this]() {
    QString path = QFileDialog::getOpenFileName(
        this,
        "Ouvrir un fichier",
        "",
        "Modèles 3D (*.obj *.fbx);;Tous les fichiers (*)"
    );
    if (!path.isEmpty()) {
        mEngine->getSceneController()->loadBlankScene();
        AssetImporter::loadAssimpScene(path.toStdString(),mEngine->getSceneController()->getScene());
        mRenderWidget->setHasChanged(true);
    }
});
    //connect(mRenderWidget->getTimer,SIGNAL(timeout()),this,timeoutControl());
}

MainWindow::~MainWindow() = default;


void MainWindow::keyPressEvent(QKeyEvent *event) {
    mPressedKeys.insert(event->key());
    switch (event->key()) {
        case(Qt::Key_P):
            qDebug() << "RenderDoc : Capture déclenchée depuis le RenderWidget !";

            // 1. On arme RenderDoc. Il va capturer la TOUTE PROCHAINE image dessinée.
            RDOC_TRIGGER_CAPTURE();

            // 2. On ordonne à Qt de redessiner l'écran immédiatement.
            // Cela va appeler ta fonction paintGL() et RenderDoc l'enregistrera !
            update();
            break;
        case(Qt::Key_1):
            mRenderWidget->getRenderer()->changeMode(ViewportMode::SOLID);
            mRenderWidget->update();
            break;
        case(Qt::Key_2):
            mRenderWidget->getRenderer()->changeMode(ViewportMode::WIREFRAME);
            mRenderWidget->update();
            break;
        case(Qt::Key_R):
            mRenderWidget->update();
            break;
#ifdef TEST_HALFEDGES
        case(Qt::Key_N):

            mRenderWidget->getRenderer()->addTestHalfEdge(1);
            mRenderWidget->update();
            break;
        case(Qt::Key_B):
            mRenderWidget->getRenderer()->addTestHalfEdge(-1);
            mRenderWidget->update();
            break;
#endif

            /*
        case(Qt::Key_Z):
            mRenderWidget->getRenderer()->getEngineCamera().strafeCamera(0.0,-1.0);
            mRenderWidget->update();
            break;
        case(Qt::Key_Q):
            mRenderWidget->getRenderer()->getEngineCamera().strafeCamera(1.0,0.0);
            mRenderWidget->update();
            break;
        case(Qt::Key_S):
            mRenderWidget->getRenderer()->getEngineCamera().strafeCamera(0.0,1.0);
            mRenderWidget->update();
            break;
        case(Qt::Key_D):
            mRenderWidget->getRenderer()->getEngineCamera().strafeCamera(-1.0,0.0);
            mRenderWidget->update();
            break;*/
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    mPressedKeys.remove(event->key());
}
void MainWindow::timeoutControl(){
    float speed = 0.05f;

    if (mPressedKeys.contains(Qt::Key_Q))
        mRenderWidget->getRenderer()->getEngineCamera().strafeCamera(-speed, 0.0f);
    if (mPressedKeys.contains(Qt::Key_D))
        mRenderWidget->getRenderer()->getEngineCamera().strafeCamera( speed, 0.0f);
    if (mPressedKeys.contains(Qt::Key_Z))
        mRenderWidget->getRenderer()->getEngineCamera().strafeCamera(0.0f,  speed);
    if (mPressedKeys.contains(Qt::Key_S))
        mRenderWidget->getRenderer()->getEngineCamera().strafeCamera(0.0f, -speed);

    mRenderWidget->update();
}