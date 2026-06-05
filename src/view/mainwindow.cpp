#include "view/mainwindow.h"
#include "view/RenderWidget.h"
#include <QOpenGLFunctions>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle(QString::fromUtf8("MiraDream3D"));
    int fps=1;//TODO enlever cette merde hardcodee

    mRenderWidget = new RenderWidget(fps,this);

    // When OpenGL funcs are ready, give scene to the RenderWidget
    mRenderWidget->show();

}

MainWindow::~MainWindow() = default;
