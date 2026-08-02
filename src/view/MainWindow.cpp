#include "view/MainWindow.h"

#include <iostream>
#include <qboxlayout.h>

#include "view/RenderWidget.h"
#include "RenderDocHelper.hpp"
#include <QFileDialog>

#include "view/EditorToolBar.h"
#include "view/HeaderBar.h"
#include "view/ViewportLayout.h"

class QHBoxLayout;
extern uint32_t frame;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    {
    setWindowTitle(QString::fromUtf8("MiraDream3D"));
    int fps=60;//TODO enlever cette merde hardcodee


    mRenderWidget = new RenderWidget(fps,this);
    setCentralWidget(mRenderWidget);
    this->resize(800, 600);
    connect(mRenderWidget, &RenderWidget::setViewportMode, this, &MainWindow::changedViewportMode);

    mEditorToolBar = new EditorToolBar(this);
    this->addToolBar(Qt::LeftToolBarArea, mEditorToolBar);
    connect(mEditorToolBar, &EditorToolBar::clickedEditorTool, this, &MainWindow::changedEditorTool);

    mHeaderBar = new HeaderBar(this);
    this->setMenuBar(mHeaderBar);
    connect(mHeaderBar, &HeaderBar::clickedImport, this, &MainWindow::importSceneRequested);
    connect(mHeaderBar, &HeaderBar::clickedClearScene, this, &MainWindow::clearSceneRequested);
    connect(mHeaderBar, &HeaderBar::clickedToggleGrid, this, &MainWindow::toggleGridRequested);
    connect(this, &MainWindow::onToggledGrid, mHeaderBar, &HeaderBar::onToggledGrid);

    mViewportLayout = new ViewportLayout(mRenderWidget);
    auto *overlayLayout = new QVBoxLayout(mRenderWidget);
    overlayLayout->setContentsMargins(4, 4, 4, 4); // 10px from the edges
    overlayLayout->addWidget(mViewportLayout, 0, Qt::AlignTop | Qt::AlignRight);

    connect(mViewportLayout, &ViewportLayout::clickedViewportMode, this, &MainWindow::changedViewportMode);
    connect(mRenderWidget, &RenderWidget::setViewportMode, mViewportLayout, &ViewportLayout::onViewportChange);

    // When OpenGL funcs are ready, give scene to the RenderWidget
    mRenderWidget->show();

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
