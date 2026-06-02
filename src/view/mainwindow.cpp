#include "view/mainwindow.h"
#include "view/RenderWidget.h"
#include <QOpenGLFunctions>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle(QString::fromUtf8("MiraDream3D"));
    int fps=60;//TODO enlever cette merde hardcodee

    mEngine = new Engine();
    mRenderWidget = new RenderWidget(mEngine->getScene(), fps,this);

    mRenderWidget->show();

}

MainWindow::~MainWindow() = default;
