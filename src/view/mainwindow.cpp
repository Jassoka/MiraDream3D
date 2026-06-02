#include "view/mainwindow.h"
#include "view/RenderWidget.h"
#include <QOpenGLFunctions>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle(QString::fromUtf8("MiraDream3D"));
    int fps=60;//TODO enlever cette merde hardcodee

    mEngine = new Engine();
    mRenderWidget = new RenderWidget(fps,this);

    // When OpenGL funcs are ready, give scene to the RenderWidget
    connect(mRenderWidget, &RenderWidget::initRenderer,
            this, [this](QOpenGLFunctions* glFuncs, float ratio) {
        mRenderWidget->setRenderer(new Renderer(mEngine->getScene(), this->width()/this->height(), QOpenGLContext::currentContext()->functions()));
    });

    mRenderWidget->show();

}

MainWindow::~MainWindow() = default;
