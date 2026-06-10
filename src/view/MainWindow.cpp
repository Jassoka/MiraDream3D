#include "view/MainWindow.h"

#include <iostream>

#include "view/RenderWidget.h"
#include "RenderDocHelper.hpp"
#include <QFileDialog>
#include <QToolBar>
#include <QPushButton>

extern uint32_t frame;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    {
    setWindowTitle(QString::fromUtf8("MiraDream3D"));
    int fps=60;//TODO enlever cette merde hardcodee

    mImportButton = new QPushButton("&Import",this);
    QToolBar* toolbar = addToolBar("Outils");
    toolbar->addWidget(mImportButton);



    mRenderWidget = new RenderWidget(fps,this);
    setCentralWidget(mRenderWidget);
    this->resize(800, 600);




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
        emit importSceneRequested(path.toStdString());
    }
});
    //connect(mRenderWidget->getTimer,SIGNAL(timeout()),this,timeoutControl());
}

MainWindow::~MainWindow() = default;

/*
void MainWindow::keyPressEvent(QKeyEvent *event)
{
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
    }
}
*/
