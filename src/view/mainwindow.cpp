#include "view/mainwindow.h"

#include <iostream>

#include "view/RenderWidget.h"
#include <QOpenGLFunctions>

#include "RenderDocHelper.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle(QString::fromUtf8("MiraDream3D"));
    int fps=1;//TODO enlever cette merde hardcodee

    mRenderWidget = new RenderWidget(fps,this);

    // When OpenGL funcs are ready, give scene to the RenderWidget
    mRenderWidget->show();
    mRenderWidget->resize(100,100);
}

MainWindow::~MainWindow() = default;


void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_P) {
        std::cout << "TEST C++ PUR : Touche P pressee !" << std::endl;
        qDebug() << "RenderDoc : Capture déclenchée depuis le RenderWidget !";

        // 1. On arme RenderDoc. Il va capturer la TOUTE PROCHAINE image dessinée.
        RDOC_TRIGGER_CAPTURE();

        // 2. On ordonne à Qt de redessiner l'écran immédiatement.
        // Cela va appeler ta fonction paintGL() et RenderDoc l'enregistrera !
        update();

    } else {
        // Laisse le comportement normal pour les autres touches
        MainWindow::keyPressEvent(event);
    }
}
